//
// Created by Tyler Gilbert on 12/17/21.
//

#include <design.hpp>

#include "Editor.hpp"
#include "designlab/fonts/FontAwesomeIcons.hpp"
#include "extras/Extras.hpp"

Editor::Editor(Data &data) {
  construct_object(data.cast_as_name());
  clear_flag(Flags::scrollable)
    .add_style("container")
    .add(Column(Names::first_child_column)
           .fill()
           .add(HeaderRow(
             data.title,
             icons::fa::plus_solid,
             data.add_button_text,
             show_add_form))
           .add(HorizontalLine())
           .add(Column(Names::show_column).fill_width().set_flex_grow()));

  show_values(data);
  configure_form_input(data);
}

void Editor::show_values(Data &data) {
  Model::Scope model_scope;
  auto column = find<Column>(Names::show_column);
  column.clean();
  const auto item_array
    = model().project_settings.to_object().at(data.form_name).to_array();

  if (item_array.count() == 0) {
    column.add(
      SubSectionHeading(data.nothing_to_show).set_flex_grow().center());
    return;
  }

  size_t offset = 0;
  for (const auto &item : item_array) {
    auto &info_card_data = InfoCard::Data::create();

    const auto title = var::KeyString(data.get_info_title_callback(item));

    info_card_data.set_title(title)
      .set_edit_callback(edit_info_card)
      .set_remove_callback(remove_info_card)
      .set_offset(offset);

    ++offset;

    const auto feature_list = data.get_feature_list_callback(item);
    for (const auto &feature : feature_list) {
      info_card_data.push_feature(feature);
    }

    column.add(InfoCard(info_card_data));
  }
}

void Editor::configure_form_input(Data &data) {
  Model::Scope model_scope;
  add(Container(Names::input_form_container)
        .fill()
        .add(Column(Names::input_form_column).fill()));

  auto input_form_column = find<Column>(Names::input_form_column);

  data.input_form_schema = data.get_schema_callback();

  input_form_column.add(FormHeaderRow(data.form_title, hide_form))
    .add(HorizontalLine())
    .add(Form(data.form_name, data.input_form_schema.to_object()));

  input_form_column.find<Form>(data.form_name)
    .add(AddButtonRow(
      Names::add_button,
      icons::fa::plus_solid,
      "Add",
      submit_form_add))
    .add(AddButtonRow(
      Names::edit_button,
      icons::fa::edit_solid,
      "Edit",
      submit_form_edit));

  find<Container>(Names::input_form_container)
    .add_style("slide_over_from_right_hidden")
    .add_style("slide_over_from_right", State::user1);
}

void Editor::submit_form_add(lv_event_t *e) {
  Model::Scope model_scope;
  auto *data = get_data(e);
  auto self = get_self(e);

  api::ErrorScope error_scope;
  const auto form = self.find<Form>(data->form_name);
  API_ASSERT(form.is_valid());
  model()
    .project_settings.append_form_entry(form)
    .update_dirty_bits(data->form_name)
    .save();

  if (is_error()) {
    printer().object("error", error());
    printer().key("badInput", model().project_settings.bad_key());
  } else {
    const auto is_valid
      = data->validate_callback
        && (data->validate_callback(self.find<Form>(data->form_name)) == IsValid::yes);

    if (!data->validate_callback || is_valid) {
      hide_form(e);
      self.show_values(*data);
    }
  }
}

void Editor::submit_form_edit(lv_event_t *e) {
  auto *data = get_data(e);
  auto self = get_self(e);

  const auto form = self.find<Form>(data->form_name);

  {
    Model::Scope model_scope;
    model()
      .project_settings.edit_from_form_entry(data->edit_offset, form)
      .update_dirty_bits(data->form_name)
      .save();
  }

  const auto is_valid
    = data->validate_callback
      && (data->validate_callback(self.find<Form>(data->form_name)) == IsValid::yes);

  if (!data->validate_callback || is_valid) {
    hide_form(e);
    self.show_values(*data);
  }
}

void Editor::edit_info_card(lv_event_t *e) {
  auto self = get_self(e);

  auto *data = get_data(e);
  auto *info_card_data = InfoCard::get_data_from_event(e);
  if (data->edit_clicked_callback != nullptr) {
    data->edit_clicked_callback(e, info_card_data->offset);
    return;
  }

  self.set_action(Action::edit);
  self.show_form();
  // this is clicked from an info card

  data->edit_offset = info_card_data->offset;

  {
    // populate the form with the current values
    Model::Scope model_scope;
    get_self(e)
      .find<Form>(data->form_name)
      .set_values(model()
                    .project_settings.to_object()
                    .at(data->form_name)
                    .to_array()
                    .at(data->edit_offset));
  }
}

void Editor::remove_info_card(lv_event_t *e) {
  auto *data = get_data(e);
  remove_entry(data->form_name, InfoCard::get_data_from_event(e)->offset);
  get_self(e).show_values(*data);
}

void Editor::show_add_form(lv_event_t *e) {
  auto self = get_self(e);

  auto * data = self.user_data<Data>();
  if( data->add_clicked_callback != nullptr ){
    data->add_clicked_callback(e);
    return;
  }

  self.set_action(Action::add);
  self.show_form();
}

void Editor::remove_entry(const char *key_name, u32 offset) {
  Model::Scope model_scope;
  auto array = model().project_settings.to_object().at(key_name).to_array();
  if (offset < array.count()) {
    array.remove(offset);
  }
  printf("Update dirty bits for %s\n", key_name);
  model().project_settings.update_dirty_bits(key_name).save();
}

Editor Editor::get_self(lv_event_t *e) {
  const auto target = Event(e).target();
  auto column_parent = target.find_parent<Column>(Names::first_child_column);
  auto form_container_parent
    = target.find_parent<Column>(Names::input_form_container);
  auto editor = column_parent.is_valid() ? column_parent.get_parent()
                                         : form_container_parent.get_parent();
  return editor.get<Editor>();
}

Editor &Editor::refresh_values() {
  show_values(*user_data<Data>());
  return *this;
}
