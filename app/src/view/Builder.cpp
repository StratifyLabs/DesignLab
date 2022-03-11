//
// Created by Tyler Gilbert on 2/24/22.
//

#include <json.hpp>
#include <var.hpp>

#include <design/extras/CheckList.hpp>
#include <design/extras/DrawerStack.hpp>
#include <design/extras/FileSystemCard.hpp>
#include <design/extras/Form.hpp>
#include <design/extras/NotificationToast.hpp>
#include <design/extras/Prompt.hpp>
#include <design/extras/Utility.hpp>

#include "extras/Extras.hpp"

#include "Builder.hpp"

#include "extras/ComponentTree.hpp"
#include "extras/EditComponent.hpp"

Builder::Builder(const char *name) {
  auto &data = Data::create(name);

  data.highlight_style = Style()
                           .set_border_width(2)
                           .set_border_opacity(Opacity::x50)
                           .set_border_color(Color::red());

  construct_object(data.cast_as_name());
  fill();

  add(Column(Names::container).fill());

  clear_flag(Flags::scrollable);
  add(
    Column(ViewObject::Names::content_container)
      .add_style("container")
      .add_flag(Flags::event_bubble)
      .fill()
      .add(
        Row()
          .fill_width()
          .add_flag(Flags::event_bubble)
          .add(Button(Names::back_button)
                 .add_label_as_static(icons::fa::chevron_left_solid)
                 .add_style("btn_outline_primary")
                 .add_flag(Flags::event_bubble))
          .add(ScreenHeading("Builder").set_flex_grow())
          .add(
            Label(Names::currently_selected_label).set_text_as_static("<none>"))
          .add(Button(Names::get_previous_sibling_button)
                 .add_style("btn_outline_primary")
                 .add_label_as_static(icons::fa::chevron_left_solid)
                 .add_event_callback(
                   EventCode::clicked,
                   get_previous_sibling_clicked))
          .add(Button(Names::get_parent_button)
                 .add_style("btn_outline_primary")
                 .add_label_as_static(icons::fa::chevron_up_solid)
                 .add_event_callback(EventCode::clicked, get_parent_clicked))
          .add(
            Button(Names::get_next_sibling_button)
              .add_style("btn_outline_primary")
              .add_label_as_static(icons::fa::chevron_right_solid)
              .add_event_callback(EventCode::clicked, get_next_sibling_clicked))
          .add(Button()
                 .add_style("btn_outline_primary")
                 .add_label_as_static(icons::fa::tree_solid)
                 .add_event_callback(EventCode::clicked, tree_clicked))
          .add(Button()
                 .add_label_as_static(icons::fa::plus_solid)
                 .add_event_callback(EventCode::clicked, add_clicked))
          .add(Button()
                 .add_label_as_static(icons::fa::pencil_alt_solid)
                 .add_event_callback(EventCode::clicked, edit_clicked))
          .add(Button(Names::remove_button)
                 .add_label_as_static(icons::fa::times_solid)
                 .add_style("btn_danger")
                 .add_event_callback(EventCode::clicked, remove_clicked)))
      .add(HorizontalLine())
      .add(NakedContainer(Names::target_object)
             .fill_width()
             .set_flex_grow()
             .add_flag(Flags::clickable)
             .add_event_callback(EventCode::clicked, target_clicked)));

  add(NakedContainer(Names::highlight_object)
        .fill()
        .set_height(100)
        .clear_flag(Flags::clickable)
        .add_flag(Flags::hidden)
        .set_background_opacity(Opacity::transparent)
        .set_border_opacity(Opacity::x50)
        .set_border_width(4)
        .set_border_color(Color::red()));

  data.selected_object = find(Names::target_object).object();

  auto &drawer_data = Drawer::Data::create(Names::control_drawer);

  add(Drawer(drawer_data)
        .set_height(80_percent)
        .set_width(40_percent)
        .set_open_from_right()
        .set_alignment(Alignment::right_middle)
        .close());
}

void Builder::show_clicked(lv_event_t *e) {
  auto drawer = Event(e)
                  .find_parent(ViewObject::Names::builder_object)
                  .find<Drawer>(Names::control_drawer);

  if (drawer.is_opened()) {
    drawer.close();
  } else {
    drawer.open();
  }
}

void Builder::target_clicked(lv_event_t *e) {
  auto object = Event(e).target<Generic>();
  get_builder(e).select_target(object);
}

Builder &Builder::select_target(Object object) {
  auto target = find<Generic>(Names::target_object);
  auto container = find<Generic>(ViewObject::Names::content_container);
  auto highlight_object = find<Generic>(Names::highlight_object);
  if (object.object() == highlight_object.object()) {
    return *this;
  }

  target.update_layout();

  const auto coords = object.get_coordinates();
  const auto target_coords = target.get_coordinates();
  const auto container_coords = container.get_coordinates();

  const auto highlight_point
    = coords.get_point() - container_coords.get_point();

  highlight_object.set_position(highlight_point)
    .set_size(coords.get_size())
    .clear_flag(Flags::hidden);

  data()->selected_object = object.object();

  auto current = object;
  if (current.object() != target.object()) {
    data()->json_path = object.name();
    current = current.get_parent<Generic>();
  } else {
    data()->json_path = "";
  }

  while (current.object() != target.object()) {
    data()->json_path
      = var::StringView(current.name()) + "/" + data()->json_path;
    current = current.get_parent<Generic>();
  }

  find<Label>(Names::currently_selected_label).set_text(object.name());

  printf("JSON path is -%s-\n", data()->json_path.cstring());
  return *this;
}

Builder Builder::get_builder(lv_event_t *e) {
  auto result
    = Event(e).find_parent<Builder>(ViewObject::Names::builder_object);
  if (result.is_valid() == false) {
    return screen().find<Builder>(ViewObject::Names::builder_object);
  }
  return result;
}

void Builder::add_component_clicked(lv_event_t *e) {
  if (Form::is_submit_button(e)) {
    printf("Form Submitted -- Add Component\n");
    auto add_component = Event(e).current_target<AddComponent>();
    const auto form_value = add_component.get_form().get_json_object();
    Model::Scope ms;
    printer().object("form", form_value);
    get_builder(e).add_component(form_value);
    screen().find<Modal>(Names::add_component_modal).close(300_milliseconds);
    return;
  }

  if (AddComponent::is_cancel_button(e)) {
    screen().find<Modal>(Names::add_component_modal).close(300_milliseconds);
  }
}

JsonObject Builder::get_active_json_object() const {
  if (data()->json_path.is_empty()) {
    return data()->json_tree;
  }
  return data()->json_tree.find(data()->json_path).to_object();
}

Builder &Builder::add_component(JsonObject form_value) {

  const auto type
    = form_value.at(AddComponent::Fields::component_type).to_string_view();

  const auto name
    = form_value.at(AddComponent::Fields::component_name).to_cstring();
  auto container = Container(data()->selected_object);

  printf("Selected is %s\n", data()->json_path.cstring());

  auto object = get_active_json_object();

  object.insert(name, form_value);
  {
    Model::Scope ms;
    printer().object("tree", data()->json_tree);
  }

  if (type == Components::container) {
    container.add(Container(name));
  } else if (type == Components::bar) {
    container.add(Bar(name));
  } else if (type == Components::button) {
    container.add(Button(name).add_label(
      form_value.at(AddComponent::Fields::component_label)
        .to_cstring()));
  } else if (type == Components::calendar) {
    container.add(Calendar(name));
  } else if (type == Components::canvas) {
    container.add(Canvas(name));
  } else if (type == Components::chart) {
    container.add(Chart(name));
  } else if (type == Components::color_wheel) {
    container.add(ColorWheel(name));
  } else if (type == Components::image) {
    container.add(Image(name));
  } else if (type == Components::keyboard) {
    container.add(Keyboard(name));
  } else if (type == Components::label) {
    container.add(Label(name).set_text_as_static(
      form_value.at(AddComponent::Fields::component_label).to_cstring()));
  } else if (type == Components::list) {
    container.add(List(name));
  } else if (type == Components::meter) {
    container.add(Meter(name));
  } else if (type == Components::roller) {
    container.add(Roller(name));
  } else if (type == Components::slider) {
    container.add(Slider(name));
  } else if (type == Components::spinbox) {
    container.add(SpinBox(name));
  } else if (type == Components::spinner) {
    container.add(Spinner(name));
  } else if (type == Components::switch_) {
    container.add(Switch(name));
  } else if (type == Components::table) {
    container.add(Table(name));
  } else if (type == Components::tileview) {
    container.add(TileView(name));
  } else if (type == Components::window) {
    container.add(Window(name));
  }

  else if (type == Components::alert) {
    // container.add(Alert(name));
  } else if (type == Components::badge) {
    container.add(Badge(name));
  } else if (type == Components::card) {
    container.add(Card(name));
  } else if (type == Components::column) {
    container.add(Column(name));
  } else if (type == Components::container) {
    container.add(Container(name));
  } else if (type == Components::drawer) {
    container.add(Drawer(Drawer::Data::create(name)));
  } else if (type == Components::heading1) {
    container.add(Heading1(name).get<Label>().set_text(
      form_value.at(Fields::component_label).to_cstring()));
  } else if (type == Components::heading2) {
    container.add(Heading2(name).get<Label>().set_text(
      form_value.at(Fields::component_label).to_cstring()));
  } else if (type == Components::heading3) {
    container.add(Heading3(name).get<Label>().set_text(
      form_value.at(Fields::component_label).to_cstring()));
  } else if (type == Components::heading4) {
    container.add(Heading4(name).get<Label>().set_text(
      form_value.at(Fields::component_label).to_cstring()));
  } else if (type == Components::horizontal_line) {
    container.add(HorizontalLine(name));
  } else if (type == Components::paragraph) {
    container.add(Paragraph(name));
  } else if (type == Components::row) {
    container.add(Row(name));
  }

  else if (type == Components::check_list) {
    container.add(HorizontalLine(name));
  } else if (type == Components::drawer_stack) {
    container.add(DrawerStack(DrawerStack::Data::create()));
  } else if (type == Components::filesystem_card) {
    container.add(FileSystemCard(FileSystemCard::Data::create(name)));
  } else if (type == Components::form_container) {
    container.add(Form(name));
  } else if (type == Components::form_heading) {
    container.add(Form::SectionHeading(name));
  } else if (type == Components::form_line_field) {
    container.add(
      Form::LineField(name)
        .set_label(
          form_value.at(Fields::component_label).to_cstring())
        .set_hint(
          form_value.at(Fields::component_hint).to_cstring()));
  } else if (type == Components::form_select) {
    container.add(
      Form::Select(name)
        .set_label(
          form_value.at(Fields::component_label).to_cstring())
        .set_hint(
          form_value.at(Fields::component_hint).to_cstring()));
  } else if (type == Components::form_file_select) {
    container.add(
      Form::SelectFile(Form::SelectFile::Data::create(name))
        .set_label(
          form_value.at(Fields::component_label).to_cstring())
        .set_hint(
          form_value.at(Fields::component_hint).to_cstring()));
  } else if (type == Components::form_switch) {
    container.add(
      Form::Switch(name)
        .set_label(
          form_value.at(Fields::component_label).to_cstring())
        .set_hint(
          form_value.at(Fields::component_hint).to_cstring()));
  }

  else {
    return *this;
  }

  auto component = container.find<Generic>(name);

  if (const auto width
      = form_value.at(AddComponent::Fields::component_width).to_string_view();
      width.is_empty() == false) {
    const auto is_percentage = width.find("%") != StringView::npos;
    const auto value = width.to_unsigned_long();
    if (is_percentage) {
      component.set_width(Percent(value).value());
    } else {
      component.set_width(value);
    }
  }

  if (const auto height
      = form_value.at(AddComponent::Fields::component_height).to_string_view();
      height.is_empty() == false) {
    const auto is_percentage = height.find("%") != StringView::npos;
    const auto value = height.to_unsigned_long();
    if (is_percentage) {
      component.set_height(Percent(value).value());
    } else {
      component.set_height(value);
    }
  }

  component.add_flag(Flags::clickable)
    .add_event_callback(EventCode::clicked, target_clicked);

  return *this;
}

void Builder::get_parent_clicked(lv_event_t *e) {
  auto builder = get_builder(e);
  auto selected = Generic(builder.data()->selected_object);
  if (selected.object() == builder.find(Names::target_object).object()) {
    return;
  }
  auto higlight_parent = Generic(builder.data()->selected_object).get_parent();
  builder.select_target(higlight_parent);
}

void Builder::get_previous_sibling_clicked(lv_event_t *e) {
  auto builder = get_builder(e);
  auto selected = Generic(builder.data()->selected_object);
  if (selected.object() == builder.find(Names::target_object).object()) {
    return;
  }
  const auto offset = selected.get_index();
  if (offset > 0) {
    builder.select_target(selected.get_parent().get_child(offset - 1));
  }
}

void Builder::get_next_sibling_clicked(lv_event_t *e) {
  auto builder = get_builder(e);
  auto selected = Generic(builder.data()->selected_object);
  if (selected.object() == builder.find(Names::target_object).object()) {
    return;
  }
  const auto offset = selected.get_index();
  auto parent = selected.get_parent();
  if (offset < parent.get_child_count() - 1) {
    builder.select_target(parent.get_child(offset + 1));
  }
}

void Builder::remove_clicked(lv_event_t *e) {
  get_builder(e).remove_selected();
}

Builder &Builder::remove_selected() {
  if (data()->selected_object == find(Names::target_object).object()) {
    return *this;
  }

  {
    {
      Model::Scope ms;
      printer().object("treeBefore", data()->json_tree);
    }
    // clean the JSON tree
    const auto parent_path = fs::Path::parent_directory(data()->json_path);
    const auto object_name = fs::Path::name(data()->json_path);
    auto parent = data()->json_tree.find(parent_path);
    parent.to_object().remove(object_name);
    {
      Model::Scope ms;
      printer().object("treeAfter", data()->json_tree);
    }
  }

  // clean the graphics
  auto selected = Generic(data()->selected_object);
  auto parent = selected.get_parent();
  selected.remove();
  return select_target(parent);
}

void Builder::add_clicked(lv_event_t *e) {
  auto builder = get_builder(e);
  Modal(Names::add_component_modal)
    .add_content(
      AddComponent(Names::add_component)
        .set_parent_item_name(Generic(builder.data()->selected_object).name())
        .add_event_callback(EventCode::clicked, add_component_clicked));
}

void Builder::edit_clicked(lv_event_t *e) {
  auto builder = get_builder(e);

  // get the JSON object associated with the target
  Modal(Names::edit_component_modal)
    .add_content(
      EditComponent(
        EditComponent::Data::create(Names::edit_component)
          .set_json_object(builder.get_active_json_object())
          .set_target_object(Generic(builder.data()->selected_object)))
        .add_event_callback(EventCode::clicked, edit_component_clicked));
}

Builder &Builder::edit_component(json::JsonObject form_value) {
  // once editing modal is done, call this to sync the new JSON object settings
  // with item visually
  auto json_object = get_active_json_object();
  auto target = Generic(data()->selected_object);

  const auto key_container = form_value.get_key_list();
  for (const auto &key : key_container) {
    const auto value = form_value.at(key);
    json_object.insert(key, value);
    const auto property = Utility::property_from_string(key);
    if (property != Property::invalid) {
      const auto description = Utility::get_property_description(property);
      const auto value_string = value.to_string_view();
      if (
        description.type == PropertyType::coordinate
        && value_string == "grow") {
        target.set_flex_grow();
      } else {
        const auto property_value
          = Utility::get_property_value(value, description.type);
        target.set_property(property, property_value);
      }
    }
  }

  Model::Scope ms;
  printer().object("activeObject", json_object);
  printer().object("tree", data()->json_tree);

  select_target(Generic(data()->selected_object));

  return *this;
}

void Builder::edit_component_clicked(lv_event_t *e) {
  if (EditComponent::is_cancel_button(e)) {
    screen().find<Modal>(Names::edit_component_modal).close(300_milliseconds);
  }

  if (Form::is_submit_button(e)) {
    printf("Form submitted\n");

    auto edit_component = Event(e).current_target<EditComponent>();
    const auto form_value = edit_component.get_form().get_json_object();

    {
      Model::Scope ms;
      printer().object("editForm", form_value);
    }

    get_builder(e).edit_component(form_value);

    screen().find<Modal>(Names::edit_component_modal).close(300_milliseconds);
  }
}

Builder &Builder::load_json_tree(Settings::Component component) {
  data()->json_tree = JsonObject();
  auto target = find<Generic>(Names::target_object);
  // draw the objects on the display and add them to the tree
  select_target(target);
  build_tree(target.remove_children().object(), component.tree());

  data()->is_new = !component.tree().is_valid();

  return *this;
}

void Builder::build_tree(lv_obj_t *lvgl_object, json::JsonObject object) {
  if (!object.is_valid() || object.is_empty()) {
    return;
  }

  data()->selected_object = lvgl_object;

  const auto key_container = object.get_key_list();

  for (const auto &key : key_container) {
    auto child_value = object.at(key);
    if (child_value.is_object()) {
      lv_obj_t *parent = data()->selected_object;

      add_component(child_value.to_object());

      auto just_added_object
        = Generic(data()->selected_object)
            .find<Generic>(
              child_value.to_object().at(Fields::component_name).to_cstring())
            .object();
      select_target(Generic(just_added_object));
      build_tree(just_added_object, child_value.to_object());

      data()->selected_object = parent;
      select_target(Generic(parent));
    }
  }
}

void Builder::tree_clicked(lv_event_t *e) {
  Modal(Names::tree_modal)
    .add_content(
      ComponentTree()
        .set_tree(get_builder(e).data()->json_tree)
        .add_event_callback(EventCode::clicked, [](lv_event_t *e) {
          screen().find<Modal>(Names::tree_modal).close(300_milliseconds);

          if (ComponentTree::is_cancel_button(e)) {
            return;
          }

          const auto name = Event(e).target().name();
          auto builder = get_builder(e);
          auto target_object = builder.find<Generic>(Names::target_object);
          auto target = target_object.find<Generic>(name);
          builder.select_target(target);
        }));
}
