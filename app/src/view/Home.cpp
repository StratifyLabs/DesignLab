//
// Created by Tyler Gilbert on 12/8/21.
//

#include "AssetMaker.hpp"
#include "ColorPreview.hpp"
#include "FontMaker.hpp"
#include "IconMaker.hpp"
#include "Project.hpp"
#include "ThemeMaker.hpp"
#include "ThemePreview.hpp"

#include "designlab/fonts/FontAwesomeIcons.hpp"

#include "Home.hpp"

void Home::configure(Generic generic) {

  generic.add(
    Row()
      .fill()
      .set_padding(0)
      .add(Container()
             .add_style("darker")
             .set_width(450)
             .fill_height()
             .set_border_side(BorderSide::right)
             .set_border_width(3)
             .add(Column().setup(configure_button_column)))
      .add(NakedContainer(Names::content_area).set_flex_grow().fill_height()));

  Project::configure(screen().find<Generic>(Names::content_area));
}

void Home::configure_button_column(Column column) {
  column.add_style("darker").fill();

  column.add(Image().set_source("a:Stratify-Icon-250px.png"));
  column
    .add(SectionHeading("Design Lab").set_text_alignment(TextAlignment::center))
    .add(HorizontalLine());

  add_side_button(
    column,
    icons::fa::folder_open_solid,
    Names::project_button,
    Project::configure);
  add_side_button(
    column,
    icons::fa::th_list_solid,
    Names::theme_button,
    ThemeMaker::configure);
  add_side_button(
    column,
    icons::fa::images_solid,
    Names::asset_button,
    AssetMaker::configure);
  add_side_button(
    column,
    icons::fa::font_solid,
    Names::font_button,
    FontMaker::configure);
  add_side_button(
    column,
    icons::fa::icons_solid,
    Names::icon_button,
    IconMaker::configure);

  column.add(NakedContainer().set_flex_grow());
  add_side_button(
    column,
    icons::fa::palette_solid,
    "Colors",
    ColorPreview::configure);
  add_side_button(
    column,
    icons::fa::th_list_solid,
    "Theme Preview",
    ThemePreview::configure);
  column.add(Label().set_text_as_static("Account"));

  column.find<Button>(Names::project_button).add_state(State::checked);
}

void Home::add_side_button(
  Column column,
  const char *icon,
  const char *name,
  void (*callback)(Generic)) {
  column.add(
    Button(name)
      .add_flag(Flags::checkable)
      .clear_state(State::checked)
      .fill_width()
      .add_style(Row::get_style())
      .set_height(Button::size_from_content)
      .set_column_padding(20)
      .set_padding(16)
      .add(Label().set_text_as_static(icon).add_style("text_color_primary"))
      .add_label_as_static(name)
      .add_style("btn_light")
      .set_background_opacity(Opacity::transparent)
      .set_background_opacity(Opacity::x70, State::checked)
      .set_background_color(Color::white(), State::checked)
      .add_event_callback(
        EventCode::clicked,
        (void *)callback,
        update_buttons));
};

void Home::update_buttons(lv_event_t *e) {
  const auto event = Event(e);
  const auto callback = event.user_data<void (*)(Generic)>();
  auto self = event.target<Button>();
  // mark as not checked
  auto button_area = self.get_parent().get<Generic>();
  for (auto i : api::Index(button_area.get_child_count())) {
    auto current = button_area.get_child(i).get<Generic>();
    if (current.object() != self.object()) {
      button_area.get_child(i).get<Generic>().clear_state(State::checked);
    }
  }

  if (callback) {
    if (self.has_state(State::checked)) {
      auto container = screen().find<Generic, IsAssertOnFail::no>(
        ViewObject::Names::content_container);
      if (container.is_valid()) {
        Event::send(container, EventCode::exited);
      }
      callback(screen().find<Generic>(Names::content_area).clean());
    } else {
      self.add_state(State::checked);
    }
  }
}
