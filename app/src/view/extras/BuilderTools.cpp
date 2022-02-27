//
// Created by Tyler Gilbert on 2/24/22.
//

#include <design.hpp>
#include <lvgl.hpp>

#include "design/extras/Form.hpp"
#include "designlab/fonts/FontAwesomeIcons.hpp"

#include "BuilderTools.hpp"

BuilderTools::BuilderTools(const char *name) {
  construct_object(name);
  fill();
  add_style("card");

  add(Column(Names::top_container).fill().add_style("container"));

  auto column = find<Column>(Names::top_container);

  column.add(
    Form(Names::form).fill_width().set_height(Form::size_from_content));
  auto form = column.find<Form>(Names::form);

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

  form.add(Form::SubmitButton()
             .set_label_as_static(ICONS_FA_PLUS_SOLID " Add")
             .add_style("success"));

  form.add_flag(Flags::event_bubble);
  column.add_flag(Flags::event_bubble);

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

  dropdown.set_options(component_options.cstring());
}

BuilderTools BuilderTools::get_builder_tools(lv_event_t *e) {
  return Event(e).find_parent(Names::top_container).get_parent<BuilderTools>();
}

void BuilderTools::form_clicked(lv_event_t *e) { printf("Form clicked\n"); }

void BuilderTools::fill_width_clicked(lv_event_t *e) {
  auto form = get_builder_tools(e).find<Form>(Names::form);
  form.set_value(form.find(Fields::component_width), "100%");
}

void BuilderTools::fill_height_clicked(lv_event_t *e) {
  auto form = get_builder_tools(e).find<Form>(Names::form);
  form.set_value(form.find(Fields::component_height), "100%");
}
