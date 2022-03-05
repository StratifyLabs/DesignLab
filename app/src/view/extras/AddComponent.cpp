//
// Created by Tyler Gilbert on 2/24/22.
//

#include <design.hpp>
#include <lvgl.hpp>
#include <var.hpp>

#include "design/extras/Form.hpp"
#include "designlab/fonts/FontAwesomeIcons.hpp"

#include "AddComponent.hpp"

AddComponent::AddComponent(const char *name) {
  construct_object(name);
  set_width(90_percent);
  set_height(90_percent);
  add_style("card");

  add(Column(Names::top_container).fill().add_style("container"));

  auto column = find<Column>(Names::top_container);

  column.add_flag(Flags::event_bubble)
    .add(
      Row(Names::main_row)
        .add_flag(Flags::event_bubble)
        .fill()
        .add(Column(Names::control_column)
               .add_event_callback(EventCode::clicked, control_button_clicked)
               .fill_height()
               .set_width(30_percent))
        .add(Column(Names::form_column)
               .fill_height()
               .add_flag(Flags::event_bubble)
               .add(Heading3(Names::parent_label, "Add to ..."))
               .set_flex_grow()
               .add(Form(Names::form)
                      .fill_width()
                      .set_height(Form::size_from_content))));

  add_type_selection(column.find<Generic>(Names::control_column));

  auto form = column.find(Names::form_column).find<Form>(Names::form);

  form.add(
    Row()
      .fill_width()
      .add(Button(Names::fill_width_button)
             .add_label_as_static(icons::fa::arrows_alt_h_solid)
             .add_style("btn_sm btn_outline_primary")
             .add_event_callback(EventCode::clicked, fill_width_clicked))
      .add(Button(Names::fill_height_button)
             .add_style("btn_sm btn_outline_primary")
             .add_label_as_static(icons::fa::arrows_alt_v_solid)
             .add_event_callback(EventCode::clicked, fill_height_clicked)));

  form.add(Form::LineField(Fields::component_name)
             .fill_width()
             .set_label_as_static("Component Name")
             .set_hint_as_static("Specify a component name"));

  form.add(Form::Select(Fields::component_type)
             .fill_width()
             .set_hint_as_static("Choose component type")
             .set_label_as_static("Type"));

  form.add(Form::LineField(Fields::component_width)
             .fill_width()
             .set_label_as_static("Width")
             .set_hint_as_static("Width in pixels or % (blank for default)"));

  form.add(Form::LineField(Fields::component_height)
             .fill_width()
             .set_label_as_static("Height")
             .set_hint_as_static("Height in pixels or % (blank for default)"));

  form.add(HorizontalLine(Names::form_hline_type_start));

  form.add(Form::LineField(Fields::component_button_label)
             .fill_width()
             .add_flag(Flags::hidden)
             .set_label_as_static("Button Label")
             .set_hint_as_static(
               "This label will be added to the center of the button"));

  form.add(Form::LineField(Fields::component_label_text)
             .fill_width()
             .add_flag(Flags::hidden)
             .set_label_as_static("Text")
             .set_hint_as_static(
               "The text to assign to the label"));

  form.add(Form::LineField(Fields::component_heading1_label)
             .fill_width()
             .add_flag(Flags::hidden)
             .set_label_as_static("Heading")
             .set_hint_as_static("Text to show as the heading"));

  form.add(Form::LineField(Fields::component_heading2_label)
             .fill_width()
             .add_flag(Flags::hidden)
             .set_label_as_static("Heading")
             .set_hint_as_static("Text to show as the heading"));

  form.add(Form::LineField(Fields::component_heading3_label)
             .fill_width()
             .add_flag(Flags::hidden)
             .set_label_as_static("Heading")
             .set_hint_as_static("Text to show as the heading"));

  form.add(Form::LineField(Fields::component_heading4_label)
             .fill_width()
             .add_flag(Flags::hidden)
             .set_label_as_static("Heading")
             .set_hint_as_static("Text to show as the heading"));

  form.add(Form::LineField(Fields::component_form_line_field_label)
             .fill_width()
             .add_flag(Flags::hidden)
             .set_label_as_static("Input Label")
             .set_hint_as_static("Label to apply to the form line field"));

  form.add(Form::LineField(Fields::component_form_line_field_hint)
             .fill_width()
             .add_flag(Flags::hidden)
             .set_label_as_static("Input Hint")
             .set_hint_as_static("Hint to apply to the form line field"));

  form.add(Form::LineField(Fields::component_form_select_label)
             .fill_width()
             .add_flag(Flags::hidden)
             .set_label_as_static("Input Label")
             .set_hint_as_static("Label to apply to the select input"));

  form.add(Form::LineField(Fields::component_form_select_hint)
             .fill_width()
             .add_flag(Flags::hidden)
             .set_label_as_static("Input Hint")
             .set_hint_as_static("Hint to apply to the select input"));

  form.add(Form::LineField(Fields::component_form_file_select_label)
             .fill_width()
             .add_flag(Flags::hidden)
             .set_label_as_static("Input Label")
             .set_hint_as_static("Label to apply to the file select input"));

  form.add(Form::LineField(Fields::component_form_file_select_hint)
             .fill_width()
             .add_flag(Flags::hidden)
             .set_label_as_static("Input Hint")
             .set_hint_as_static("Hint to apply to the file select input"));

  form.add(Form::LineField(Fields::component_form_switch_label)
             .fill_width()
             .add_flag(Flags::hidden)
             .set_label_as_static("Input Label")
             .set_hint_as_static("Label to apply to the switch input"));

  form.add(Form::LineField(Fields::component_form_switch_hint)
             .fill_width()
             .add_flag(Flags::hidden)
             .set_label_as_static("Input Hint")
             .set_hint_as_static("Hint to apply to the switch input"));

  form.add(HorizontalLine(Names::form_hline_type_stop));

  form.add(Row()
             .add_flag(Flags::event_bubble)
             .fill_width()
             .add(Form::SubmitButton()
                    .set_label_as_static(ICONS_FA_PLUS_SOLID " Add")
                    .add_style("btn_success"))
             .add(Button(Names::cancel_button)
                    .add_flag(Flags::event_bubble)
                    .add_label_as_static(ICONS_FA_TIMES_SOLID " Cancel")
                    .add_style("btn_outline_primary")));

  form.add_flag(Flags::event_bubble);

  form.add_event_callback(EventCode::clicked, form_clicked);

  auto dropdown
    = form.find<Form::Select>(Fields::component_type).get_dropdown();

  var::String component_options;
  for (const auto *option : Components::list_of_lvgl_components) {
    component_options += var::String(option) + "\n";
  }

  for (const auto *option : Components::list_of_design_components) {
    component_options += var::String(option) + "\n";
  }

  for (const auto *option : Components::list_of_extra_design_components) {
    component_options += var::String(option) + "\n";
  }

  dropdown.set_options(component_options.cstring());
}

AddComponent AddComponent::get_add_component(lv_event_t *e) {
  return Event(e).find_parent(Names::top_container).get_parent<AddComponent>();
}

void AddComponent::form_clicked(lv_event_t *e) { printf("Form clicked\n"); }

void AddComponent::fill_width_clicked(lv_event_t *e) {
  auto form = get_add_component(e).get_form();
  form.set_value(form.find(Fields::component_width), "100%");
}

void AddComponent::fill_height_clicked(lv_event_t *e) {
  auto form = get_add_component(e).get_form();
  form.set_value(form.find(Fields::component_height), "100%");
}

void AddComponent::control_button_clicked(lv_event_t *e) {
  auto target = Event(e).target();
  auto form = get_add_component(e).get_form();
  printf("Set form value for type %s\n", target.name());
  form.set_value(form.find(Fields::component_type), target.name());

  bool is_filter_active = false;
  for (auto object : form) {
    if (object.name() == Names::form_hline_type_stop) {
      is_filter_active = false;
    }

    if (object.name() == Names::form_hline_type_start) {
      is_filter_active = true;
    } else if (is_filter_active) {
      const auto target_name = StringView(target.name());
      const auto object_name = StringView(object.name());

      printf("name for filter is %s\n", target.name());
      if (
        (target_name == Components::form_container)
        || object_name.find(target_name) == StringView::npos) {
        object.get<Generic>().add_flag(Flags::hidden);
      } else {
        object.get<Generic>().clear_flag(Flags::hidden);
      }
    }
  }
}

void AddComponent::add_type_selection(lvgl::Generic generic) {

  static auto get_label = [](const ComponentType &type) {
    return KeyString(type.icon).append(" ").append(type.name).replace(
      KeyString::Replace().set_old_character('_').set_new_character(' '));
  };

  static auto add_button = [](Row row, const ComponentType &type) {
    row.add(Button(type.name)
              .add_style("btn_sm btn_outline_primary")
              .add_flag(Flags::event_bubble)
              .add_label(get_label(type)));
  };

  generic.add(Heading2("Select Type"))
    .add(HorizontalLine())
    .add(Heading4("lvgl"));

  generic.add(Row(Names::lvgl_row)
                .fill_width()
                .add_flag(Flags::event_bubble)
                .set_flex_flow(FlexFlow::row_wrap)
                .set_row_padding(20));
  {
    auto row = generic.find<Row>(Names::lvgl_row);
    for (const auto &component_type : component_type_list_lvgl) {
      add_button(row, component_type);
    }
  }

  generic.add(Heading4("design"));
  generic.add(Row(Names::design_row)
                .add_flag(Flags::event_bubble)
                .fill_width()
                .set_flex_flow(FlexFlow::row_wrap)
                .set_row_padding(20));
  {
    auto row = generic.find<Row>(Names::design_row);
    for (const auto &component_type : component_type_list_design) {
      add_button(row, component_type);
    }
  }

  generic.add(Heading4("design extras"));
  generic.add(Row(Names::extra_design_row)
                .add_flag(Flags::event_bubble)
                .fill_width()
                .set_flex_flow(FlexFlow::row_wrap)
                .set_row_padding(20));
  {
    auto row = generic.find<Row>(Names::extra_design_row);
    for (const auto &component_type : component_type_list_extra_design) {
      add_button(row, component_type);
    }
  }

  generic.add(Heading4("form items"));
  generic.add(Row(Names::extra_design_form_row)
                .add_flag(Flags::event_bubble)
                .fill_width()
                .set_flex_flow(FlexFlow::row_wrap)
                .set_row_padding(20));
  {
    auto row = generic.find<Row>(Names::extra_design_form_row);
    for (const auto &component_type : component_type_list_extra_design_form) {
      add_button(row, component_type);
    }
  }
}

bool AddComponent::is_cancel_button(lv_event_t *e) {
  return Event(e).target().name() == Names::cancel_button;
}

AddComponent &AddComponent::set_parent_item_name(const char *value) {
  const auto label = StringView("Add to ") | value;
  find<Label>(Names::parent_label).set_text(label.cstring());
  return *this;
}
