//
// Created by Tyler Gilbert on 12/12/21.
//

#include <var.hpp>

#include "Editor.hpp"
#include "designlab/fonts/FontAwesomeIcons.hpp"
#include "logic/FontManager.hpp"

#include "Builder.hpp"
#include "ComponentMaker.hpp"

ComponentMaker::ComponentMaker(const char *name) {
  construct_object(name);
  fill();

  auto &editor_data = Editor::Data::create(Names::component_maker_editor)
                        .set_add_button_text("Add Component")
                        .set_form_name(Settings::components_key())
                        .set_nothing_to_show("No Components")
                        .set_add_clicked_callback(add_clicked)
                        .set_edit_clicked_callback(edit_clicked)
                        .set_title("Components")
                        .set_form_title("Component Details")
                        .set_get_info_title_callback(get_info_title)
                        .set_get_feature_list_callback(get_feature_list)
                        .set_get_schema_callback(InputSchema::get_form_schema);

  clear_flag(Flags::scrollable).add(Editor(editor_data).fill());

  add(Drawer(Drawer::Data::create(Names::builder_drawer))
        .fill()
        .set_open_from_right()
        .add_content(
          Builder(ViewObject::Names::builder_object)
            .add_event_callback(EventCode::clicked, builder_button_clicked)));
}

var::Vector<InfoCard::Data::Feature>
ComponentMaker::get_feature_list(json::JsonObject object) {
  var::Vector<InfoCard::Data::Feature> result;
  Settings::Component item(object);

  result.push_back(
    {.icon = icons::fa::info_circle_solid,
     .label = "Name",
     .value = item.get_name()});
  return result;
}

var::StringView ComponentMaker::get_info_title(json::JsonObject object) {
  Settings::Component item(object);
  return fs::Path::name(item.get_name());
}
void ComponentMaker::edit_clicked(lv_event_t *e, u32 offset) {
  auto self = get_self(e);

  printf("load offset %d\n", offset);
  // which object is being edited?
  const auto component_object = [](u32 offset) {
    Model::Scope ms;
    const auto result
      = model().project_settings.components_to_array().at(offset);
    printer().object("component", result);
    return result;
  }(offset);

  Settings::Component component(component_object);

  self.find<Builder>(ViewObject::Names::builder_object)
    .set_component_offset(offset)
    .set_component_name(component.get_name())
    .load_json_tree(component);
  screen().find<Drawer>(Names::builder_drawer).open();
}

void ComponentMaker::add_clicked(lv_event_t *e) {
  auto self = get_self(e);
  self.find<Builder>(ViewObject::Names::builder_object).load_json_tree({});
  screen().find<Drawer>(Names::builder_drawer).open();
}

void ComponentMaker::builder_button_clicked(lv_event_t *e) {
  if (Builder::is_back_button(e)) {
    // close the builder drawer
    Event(e).find_parent<Drawer>(Names::builder_drawer).close();

    auto builder = get_self(e).find<Builder>(ViewObject::Names::builder_object);
    const auto is_new = builder.is_new();

    if (!is_new) {

      Model::Scope ms;

      const auto component = Settings::Component()
                               .set_name(builder.component_name())
                               .set_tree(builder.get_json_tree())
                               .trim_tree();

      auto components_array = model().project_settings.components_to_array();
      components_array.remove(builder.component_offset());
      components_array.insert(builder.component_offset(), component);
      model().project_settings.save();

    } else {
      Modal(Names::new_component_modal)
        .add_content(
          Prompt(
            Prompt::Data::create(Names::new_component_prompt)
              .set_title("Add New Component")
              .set_accept("OK")
              .set_accept_callback([](lv_event_t *) {
                auto form = screen().find<Form>(
                  {Names::new_component_prompt, Names::new_component_form});

                auto builder
                  = screen().find<Builder>(ViewObject::Names::builder_object);

                {
                  Model::Scope ms;
                  const auto component
                    = Settings::Component()
                        .set_name(form.get_json_object()
                                    .at(Names::new_component_line_field_name)
                                    .to_string_view())
                        .set_tree(builder.get_json_tree())
                        .trim_tree();

                  model().project_settings.components_to_array().append(
                    component);
                  model().project_settings.save();
                }

                screen()
                  .find<Modal>(Names::new_component_modal)
                  .close(300_milliseconds);

                screen()
                  .find<Editor>(Names::component_maker_editor)
                  .refresh_values();
              })
              .set_message("")
              .set_reject("Cancel")
              .set_reject_callback([](lv_event_t *) {
                screen()
                  .find<Modal>(Names::new_component_modal)
                  .close(300_milliseconds);
              }))
            .set_width(80_percent)
            .set_height(40_percent))
        .set_enabled();

      auto prompt = screen().find<Prompt>(Names::new_component_prompt);
      prompt.get_content_column().add(
        Form(Names::new_component_form)
          .add(Form::LineField(Names::new_component_line_field_name)
                 .set_label_as_static("Component Name")
                 .set_hint_as_static("Assign a name to this component (class "
                                     "name when generating code)")));

      prompt.find<Form>(Names::new_component_form)
        .move_to_index(
          prompt.get_message_label().add_flag(Flags::hidden).get_index());
    }
  }
}
ComponentMaker ComponentMaker::get_self(lv_event_t *e) {
  auto self = Event(e).find_parent<ComponentMaker>(
    ViewObject::Names::component_maker_object);
  if (self.is_valid() == false) {
    return screen().find<ComponentMaker>(
      ViewObject::Names::component_maker_object);
  }
  return self;
}

ComponentMaker::InputSchema::InputSchema() {
  Model::Scope model_scope;
  push_back(Form::SelectFile::Schema()
              .set_name(Settings::Font::path_key())
              .set_base_path(model().session_settings.get_project())
              .set_label("Select Font File")
              .set_hint("Choose the ttf file to use."));

  push_back(Form::LineField::Schema()
              .set_name(Settings::Font::name_key())
              .set_label("Name")
              .set_value("montserrat")
              .set_hint("The name of the font (should match the file)."));
  push_back(
    Form::Select::Schema()
      .set_name(Settings::Font::style_key())
      .set_label("Style")
      .set_value("regular")
      .set_options("any\nthin\nthin_italic\nextra_light\nextra_light_"
                   "italic\nlight\nlight_italic\nregular\nregular_"
                   "italic\nmedium\nmedium_italic\nsemi_bold\nsemi_bold_"
                   "italic\nbold\nbold_italic\nextra_bold\nextra_bold_italic")
      .set_hint("The style of the font (should match the file)."));
}
