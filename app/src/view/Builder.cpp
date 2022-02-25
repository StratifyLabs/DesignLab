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
                 .add_label_as_static(ICONS_FA_HAMMER_SOLID " Tools")
                 .add_event_callback(EventCode::clicked, show_clicked)))
      .add(HorizontalLine())
      .add(NakedContainer(Names::target_object)
             .fill_width()
             .set_flex_grow()
             .add_style(data.highlight_style, State::user4)
             .add_flag(Flags::clickable)
             .add_event_callback(EventCode::clicked, target_clicked)));

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

  auto target = get_builder(e).find<Generic>(Names::target_object);
  // recurse into target and remove user4
  if (target.object() != object.object()) {
    target.clear_state(State::user4);
  }
  for (auto child : target) {
    if (child.object() != object.object()) {
      child.get<Generic>().clear_state(State::user4);
    }
  }

  get_builder(e).data()->selected_object = object.object();
  if (object.has_state(State::user4)) {
    object.clear_state(State::user4);
  } else {
    object.add_state(State::user4);
  }
}

Builder Builder::get_builder(lv_event_t *e) {
  return Event(e).find_parent<Builder>(ViewObject::Names::builder_object);
}

BuilderTools Builder::get_builder_tools() const {
  return find<BuilderTools>(Names::builder_tools);
}

void Builder::builder_tools_clicked(lv_event_t *e) {
  printf("builder tools clicked\n");
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

void Builder::update_target_properties(Generic generic) {
  generic.add_flag(Flags::clickable)
    .add_event_callback(EventCode::clicked, target_clicked)
    .add_style(data()->highlight_style, State::user4);
}

Builder &Builder::add_label(const char *name) {
  if (data()->selected_object != nullptr) {
    auto container = Container(data()->selected_object);
    container.add(Label(name).set_text_as_static("New Label"));
    update_target_properties(container.find<Generic>(name));
  }

  return *this;
}

Builder &Builder::add_row() { return *this; }

Builder &Builder::add_component(json::JsonObject form_value) {
  const auto type
    = form_value.at(BuilderTools::Fields::component_type).to_string_view();
  const auto name
    = form_value.at(BuilderTools::Fields::component_name).to_cstring();
  auto container = Container(data()->selected_object);

  if (type == BuilderTools::Components::container) {
    container.add(Container(name));
  } else if (type == BuilderTools::Components::button) {
    container.add(Button(name).add_label_as_static("Button"));
  } else if (type == BuilderTools::Components::label) {
    container.add(Label(name).set_text_as_static("New Label"));
  } else if (type == BuilderTools::Components::row) {
    container.add(Row(name));
  } else if (type == BuilderTools::Components::column) {
    container.add(Column(name));
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
  printf("Get parent of selected object\n");
}

void Builder::get_previous_sibling_clicked(lv_event_t *e) {
  printf("Get previous sibling\n");
}

void Builder::get_next_sibling_clicked(lv_event_t *e) {
  printf("Get next sibling\n");
}
