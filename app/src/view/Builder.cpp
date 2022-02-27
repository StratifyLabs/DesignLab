//
// Created by Tyler Gilbert on 2/24/22.
//

#include <var.hpp>

#include "extras/Extras.hpp"

#include "Builder.hpp"
#include "extras/BuilderTools.hpp"

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
      .fill()
      .add(
        Row()
          .fill_width()
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
                 .add_label_as_static(icons::fa::plus_solid)
                 .add_event_callback(EventCode::clicked, show_clicked))
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
        .add(BuilderTools(Names::builder_tools)
               .add_event_callback(EventCode::clicked, builder_tools_clicked))
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
  return Event(e).find_parent<Builder>(ViewObject::Names::builder_object);
}

BuilderTools Builder::get_builder_tools() const {
  return find<BuilderTools>(Names::builder_tools);
}

void Builder::builder_tools_clicked(lv_event_t *e) {
  if (Form::is_submit_button(e)) {
    printf("Form Submitted -- Add Component\n");

    auto builder_tools = Event(e).current_target<BuilderTools>();
    const auto form_value
      = builder_tools.get_add_component_form().get_json_object();
    Model::Scope ms;
    printer().object("form", form_value);

    get_builder(e).add_component(form_value);
    return;
  }
}

Builder &Builder::add_component(json::JsonObject form_value) {
  const auto type
    = form_value.at(BuilderTools::Fields::component_type).to_string_view();
  const auto name
    = form_value.at(BuilderTools::Fields::component_name).to_cstring();
  auto container = Container(data()->selected_object);

  printf("Selected is %s\n", data()->json_path.cstring());
  auto object = [&]() {
    if (data()->json_path.is_empty()) {
      return data()->json_tree;
    }
    return data()->json_tree.find(data()->json_path).to_object();
  }();

  object.insert(name, form_value);
  {
    Model::Scope ms;
    printer().object("tree", data()->json_tree);
  }

  if (type == BuilderTools::Components::container) {
    container.add(Container(name));
  } else if (type == BuilderTools::Components::button) {
    container.add(Button(name));
  } else if (type == BuilderTools::Components::label) {
    container.add(Label(name).set_text_as_static("New Label"));
  } else if (type == BuilderTools::Components::row) {
    container.add(Row(name));
  } else if (type == BuilderTools::Components::column) {
    container.add(Column(name));
  } else if (type == BuilderTools::Components::heading1) {
    container.add(Heading1(name));
  } else if (type == BuilderTools::Components::heading2) {
    container.add(Heading2(name));
  } else if (type == BuilderTools::Components::heading3) {
    container.add(Heading3(name));
  } else if (type == BuilderTools::Components::heading4) {
    container.add(Heading4(name));
  } else if (type == BuilderTools::Components::horizontal_line) {
    container.add(HorizontalLine(name));
  } else if (type == BuilderTools::Components::image) {
    container.add(Image(name));
  } else if (type == BuilderTools::Components::spinner) {
    container.add(Spinner(name));
  } else if (type == BuilderTools::Components::meter) {
    container.add(Meter(name));
  } else if (type == BuilderTools::Components::spinbox) {
    container.add(SpinBox(name));
  } else {
    return *this;
  }

  auto component = container.find<Generic>(name);

  if (const auto width
      = form_value.at(BuilderTools::Fields::component_width).to_string_view();
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
      = form_value.at(BuilderTools::Fields::component_height).to_string_view();
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
    .add_event_callback(EventCode::clicked, target_clicked)
    .add_style(data()->highlight_style, State::user4);

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
  auto selected = Generic(data()->selected_object);
  auto parent = selected.get_parent();
  selected.remove();
  return select_target(parent);
}

void Builder::edit_clicked(lv_event_t *e) {}
