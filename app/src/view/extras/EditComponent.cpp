//
// Created by Tyler Gilbert on 3/1/22.
//

#include <design.hpp>
#include <lvgl.hpp>
#include <var.hpp>

#include <design/extras/Utility.hpp>

#include "EditComponent.hpp"

EditComponent::EditComponent(Data &data) {
  API_ASSERT(data.target_object.is_valid());
  API_ASSERT(data.json_object.is_valid());

  construct_object(data.cast_as_name());
  set_width(90_percent);
  set_height(90_percent);
  add_style("card");

  add(Column(Names::top_container).fill().add_style("container"));

  auto column = find<Column>(Names::top_container);

  const auto heading_text = StringView("Edit ") | data.target_object.name();

  column.add_flag(Flags::event_bubble)
    .add(
      Row(Names::main_row)
        .add_flag(Flags::event_bubble)
        .fill()
        .add(Column(Names::control_column).fill_height().set_width(40_percent))
        .add(
          Column(Names::form_column)
            .fill_height()
            .set_flex_grow()
            .add_flag(Flags::event_bubble)
            .add(
              Heading3(Names::parent_label).get<Label>().set_text(heading_text))
            .add(Form(Names::form)
                   .fill_width()
                   .set_height(Form::size_from_content))
            .add(NakedContainer().fill_width().set_flex_grow())));

  auto control_column = column.find<Column>(Names::control_column);

  auto form = column.find<Form>({Names::form_column, Names::form});

  const auto key_container = data.json_object.get_key_list();
  for (const auto &key : key_container) {
    add_property_to_form(form, key, -1);
  }

  form.add(HorizontalLine(Names::form_hline_type_start));
  form.add(HorizontalLine(Names::form_hline_type_stop).add_flag(Flags::hidden));

  form.add(Row()
             .fill_width()
             .add_flag(Flags::event_bubble)
             .add(Form::SubmitButton().add_style("btn_success"))
             .add(Button(Names::cancel_button)
                    .add_label_as_static(ICONS_FA_TIMES_SOLID " Cancel")
                    .add_style("btn_outline_primary")
                    .add_flag(Flags::event_bubble)));

  form.set_values(data.json_object);

  control_column.add(Heading3("Add Property"))
    .add(HorizontalLine())
    .add_event_callback(EventCode::clicked, add_property_clicked);

  for (const auto &property_group : Utility::property_group_list) {
    if (property_group != PropertyGroup::invalid) {
      const auto row_name = Utility::to_cstring(property_group);
      control_column.add(Heading4("Heading", row_name));

      control_column.add(Row(row_name)
                           .set_flex_flow(FlexFlow::row_wrap)
                           .add_flag(Flags::event_bubble)
                           .fill_width()
                           .add_style("col"));

      auto row = control_column.find<Row>(row_name);

      for (const auto &property : Utility::property_list) {
        if (property.group == property_group) {
          const auto name = Utility::to_cstring(property.property);
          if (property.property == Property::width) {
            continue;
          }
          if (property.property == Property::height) {
            continue;
          }

          row.add(
            Button(name)
              .add_flag(Flags::event_bubble)
              .add_label(GeneralString(name).replace(GeneralString::Replace()
                                                       .set_old_character('_')
                                                       .set_new_character('.')))
              .add_style("btn_outline_primary btn_sm"));
        }
      }
    }
  }
}

void EditComponent::add_property_clicked(lv_event_t *e) {
  auto name = Event(e).target().name();
  auto edit_component = get_edit_component(e);

  edit_component.data()->json_object.insert(name, json::JsonObject());

  printf("Add Property %s\n", name);

  const auto hline_offset
    = edit_component.find(Names::form_hline_type_start).get_index();
  auto form = edit_component.find<Form>({Names::form_column, Names::form});
  add_property_to_form(form, name, hline_offset);
}

EditComponent EditComponent::get_edit_component(lv_event_t *e) {
  return Event(e).find_parent(Names::top_container).get_parent<EditComponent>();
}

bool EditComponent::is_cancel_button(lv_event_t *e) {
  return Event(e).target().name() == Names::cancel_button;
}

void EditComponent::add_property_to_form(
  design::Form form,
  var::StringView property_name,
  s32 offset) {

  const auto property = [](var::StringView property_name) {
    for (const auto &property : Utility::property_list) {
      if (property_name == Utility::to_cstring(property.property)) {
        return property;
      }
    }
    return PropertyDescription{};
  }(property_name);

  if (property.group == PropertyGroup::invalid) {
    return;
  }

  const auto name = Utility::to_cstring(property.property);
  auto name_string = GeneralString(name);

  const auto replace_underscore_with_period
    = ReplaceCharacter{.new_character = '.', .old_character = '_'};

  switch (property.type) {
  case PropertyType::number: {
    form.add(Form::LineField(Utility::to_cstring(property.property))
               .set_label(name_string(replace_underscore_with_period))
               .set_hint_as_static("Specify Number"));
  } break;
  case PropertyType::coordinate: {
    form.add(Form::LineField(Utility::to_cstring(property.property))
               .set_label(name_string(replace_underscore_with_period))
               .set_hint_as_static("Specify Coordinate"));
  } break;

  case PropertyType::color: {
    form.add(Form::LineField(Utility::to_cstring(property.property))
               .set_label(name_string(replace_underscore_with_period))
               .set_hint_as_static("Specify Color (Use Hex 0xAARRGGBB"));
  } break;

  case PropertyType::alignment: {
    const auto property_name = Utility::to_cstring(property.property);
    form.add(Form::Select(property_name)
               .set_label(name_string(replace_underscore_with_period))
               .set_hint_as_static("Select Alignment"));

    auto dropdown = form.find<Form::Select>(property_name).get_dropdown();

    String options_string;
    for (const auto alignment : Utility::alignment_list) {
      options_string += Utility::to_cstring(alignment);
      options_string += "\n";
    }
    options_string.pop_back();

    dropdown.set_options(options_string.cstring());
  } break;

  case PropertyType::direction: {
    const auto property_name = Utility::to_cstring(property.property);
    form.add(Form::Select(property_name)
               .set_label(name_string(replace_underscore_with_period))
               .set_hint_as_static("Select Direction"));

    auto dropdown = form.find<Form::Select>(property_name).get_dropdown();

    String options_string;
    for (const auto value : Utility::direction_list) {
      options_string += Utility::to_cstring(value);
      options_string += "\n";
    }
    options_string.pop_back();

    dropdown.set_options(options_string.cstring());
  } break;

  case PropertyType::base_direction: {
    const auto property_name = Utility::to_cstring(property.property);
    form.add(Form::Select(property_name)
               .set_label(name_string(replace_underscore_with_period))
               .set_hint_as_static("Select Base Direction"));

    auto dropdown = form.find<Form::Select>(property_name).get_dropdown();

    String options_string;
    for (const auto value : Utility::base_direction_list) {
      options_string += Utility::to_cstring(value);
      options_string += "\n";
    }
    options_string.pop_back();

    dropdown.set_options(options_string.cstring());
  } break;

  case PropertyType::border_side: {
    const auto property_name = Utility::to_cstring(property.property);
    form.add(Form::Select(property_name)
               .set_label(name_string(replace_underscore_with_period))
               .set_hint_as_static("Select Border Side"));

    auto dropdown = form.find<Form::Select>(property_name).get_dropdown();

    String options_string;
    for (const auto value : Utility::border_side_list) {
      options_string += Utility::to_cstring(value);
      options_string += "\n";
    }
    options_string.pop_back();

    dropdown.set_options(options_string.cstring());
  } break;

  case PropertyType::milliseconds: {
    form.add(Form::LineField(Utility::to_cstring(property.property))
               .set_label(name_string(replace_underscore_with_period))
               .set_hint_as_static("Set value in milliseconds"));
  } break;

  case PropertyType::gradient_direction: {
    const auto property_name = Utility::to_cstring(property.property);
    form.add(Form::Select(property_name)
               .set_label(name_string(replace_underscore_with_period))
               .set_hint_as_static("Select Gradient Direction"));

    auto dropdown = form.find<Form::Select>(property_name).get_dropdown();

    String options_string;
    for (const auto value : Utility::gradient_direction_list) {
      options_string += Utility::to_cstring(value);
      options_string += "\n";
    }
    options_string.pop_back();

    dropdown.set_options(options_string.cstring());
  } break;

  case PropertyType::blend_mode: {
    const auto property_name = Utility::to_cstring(property.property);
    form.add(Form::Select(property_name)
               .set_label(name_string(replace_underscore_with_period))
               .set_hint_as_static("Select Blend Mode"));

    auto dropdown = form.find<Form::Select>(property_name).get_dropdown();

    String options_string;
    for (const auto value : Utility::blend_mode_list) {
      options_string += Utility::to_cstring(value);
      options_string += "\n";
    }
    options_string.pop_back();

    dropdown.set_options(options_string.cstring());
  } break;

  case PropertyType::boolean: {
    const auto property_name = Utility::to_cstring(property.property);
    form.add(Form::Switch(property_name)
               .set_label(name_string(replace_underscore_with_period)));
  } break;

  case PropertyType::flex_align: {
    const auto property_name = Utility::to_cstring(property.property);
    form.add(Form::Select(property_name)
               .set_label(name_string(replace_underscore_with_period))
               .set_hint_as_static("Select Flex Align Main"));

    auto dropdown = form.find<Form::Select>(property_name).get_dropdown();

    String options_string;
    for (const auto value : Utility::flex_align_list) {
      options_string += Utility::to_cstring(value);
      options_string += "\n";
    }
    options_string.pop_back();

    dropdown.set_options(options_string.cstring());
  } break;

  case PropertyType::flex_flow: {
    const auto property_name = Utility::to_cstring(property.property);
    form.add(Form::Select(property_name)
               .set_label(name_string(replace_underscore_with_period))
               .set_hint_as_static("Select Flex Flow"));

    auto dropdown = form.find<Form::Select>(property_name).get_dropdown();

    String options_string;
    for (const auto value : Utility::flex_flow_list) {
      options_string += Utility::to_cstring(value);
      options_string += "\n";
    }
    options_string.pop_back();

    dropdown.set_options(options_string.cstring());
  } break;

  case PropertyType::text_decoration: {
    const auto property_name = Utility::to_cstring(property.property);
    form.add(Form::Select(property_name)
               .set_label(name_string(replace_underscore_with_period))
               .set_hint_as_static("Select Text Decoration"));

    auto dropdown = form.find<Form::Select>(property_name).get_dropdown();

    String options_string;
    for (const auto value : Utility::text_decoration_list) {
      options_string += Utility::to_cstring(value);
      options_string += "\n";
    }
    options_string.pop_back();

    dropdown.set_options(options_string.cstring());
  } break;

  case PropertyType::text_alignment: {
    const auto property_name = Utility::to_cstring(property.property);
    form.add(Form::Select(property_name)
               .set_label(name_string(replace_underscore_with_period))
               .set_hint_as_static("Select Text Alignment"));

    auto dropdown = form.find<Form::Select>(property_name).get_dropdown();

    String options_string;
    for (const auto value : Utility::text_alignment_list) {
      options_string += Utility::to_cstring(value);
      options_string += "\n";
    }
    options_string.pop_back();

    dropdown.set_options(options_string.cstring());
  } break;

  case PropertyType::font: {
    const auto property_name = Utility::to_cstring(property.property);
    form.add(Form::Select(property_name)
               .set_label(name_string(replace_underscore_with_period))
               .set_hint_as_static("Select Font"));

    auto dropdown = form.find<Form::Select>(property_name).get_dropdown();

    //need to pull the font list from what will be available
    String options_string;
    for (const auto value : Utility::text_alignment_list) {
      options_string += Utility::to_cstring(value);
      options_string += "\n";
    }
    options_string.pop_back();

    dropdown.set_options(options_string.cstring());
  } break;

  default:
    break;
  }

  if (offset > 0) {
    form.find<Generic>(name).move_to_index(offset);
  }
}
