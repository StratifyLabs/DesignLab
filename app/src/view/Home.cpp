//
// Created by Tyler Gilbert on 12/8/21.
//

#include <design/Grid.hpp>

#include "About.hpp"
#include "AssetMaker.hpp"
#include "ColorPreview.hpp"
#include "ComponentMaker.hpp"
#include "FontMaker.hpp"
#include "GettingStarted.hpp"
#include "IconMaker.hpp"
#include "Project.hpp"
#include "ThemeMaker.hpp"

#include "designlab/fonts/FontAwesomeIcons.hpp"

#include "Home.hpp"

using namespace design;
using namespace lvgl;

namespace {
struct LocalNames {
  static constexpr auto button_area = "HomeButtonArea";
  static constexpr auto content_area = "HomeContentArea";
  DESIGN_DECLARE_NAME_VALUE(project_button, Project);
  DESIGN_DECLARE_NAME_VALUE(theme_button, Themes);
  DESIGN_DECLARE_NAME_VALUE(getting_started_button, Getting Started);
  static constexpr auto icon_button = "Icons";
  static constexpr auto asset_button = "Assets";
  static constexpr auto font_button = "Fonts";
  static constexpr auto about_button = "About";
  DESIGN_DECLARE_NAME_VALUE(builder_button, Builder);
  DESIGN_DECLARE_NAME_VALUE(component_button, Components);
};

void update_buttons(lv_event_t *e) {
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
      callback(screen().find<Generic>(LocalNames::content_area).clean());
    } else {
      self.add_state(State::checked);
    }
  }
}

void add_side_button(
  Column column,
  const char *icon,
  const char *name,
  void (*callback)(Generic)) {
  auto model = ModelInScope();
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
      .add_style(model.instance.is_dark_theme ? "btn_dark" : "btn_light")
      .set_background_opacity(Opacity::transparent)
      .set_background_opacity(Opacity::x70, State::checked)
      .set_background_color(
        model.instance.is_dark_theme ? Color::black() : Color::white(),
        State::checked)
      .add_event_callback(
        EventCode::clicked,
        (void *)callback,
        update_buttons));
};

void configure_button_column(Column column) {
  column.add_style("darker").fill();

  column.add(Image().set_source("a:icon-250px.png"));
  column
    .add(SectionHeading("Design Lab").set_text_alignment(TextAlignment::center))
    .add(HorizontalLine());

  add_side_button(
    column,
    icons::fa::map_signs_solid,
    LocalNames::getting_started_button,
    GettingStarted::configure);

  add_side_button(
    column,
    icons::fa::folder_open_solid,
    LocalNames::project_button,
    Project::configure);
  add_side_button(
    column,
    icons::fa::th_list_solid,
    LocalNames::theme_button,
    ThemeMaker::configure);
  add_side_button(
    column,
    icons::fa::images_solid,
    LocalNames::asset_button,
    AssetMaker::configure);
  add_side_button(
    column,
    icons::fa::font_solid,
    LocalNames::font_button,
    FontMaker::configure);
  add_side_button(
    column,
    icons::fa::icons_solid,
    LocalNames::icon_button,
    IconMaker::configure);
  add_side_button(
    column,
    icons::fa::hammer_solid,
    LocalNames::component_button,
    ComponentMaker::configure);
  add_side_button(
    column,
    icons::fa::palette_solid,
    "Colors",
    ColorPreview::configure);

  column.add(NakedContainer().set_flex_grow());

  add_side_button(
    column,
    icons::fa::info_circle_solid,
    LocalNames::about_button,
    About::configure);

  column
    .find<Button>(
      ModelInScope().instance.is_theme_updated ? LocalNames::about_button
                                               : LocalNames::project_button)
    .add_state(State::checked);
}

void set_project_button_enabled(lv_event_t *e, bool value) {}

class HomeData : public UserDataAccess<HomeData> {
public:
  explicit HomeData(const char *name = "") : UserDataAccess<HomeData>(name) {}
  ApplicationEventBus::Subscription<HomeData> change_theme_subscription
    = ApplicationEventBus::Subscription<HomeData>(
      EventBusId::notify_home,
      this);

  void handle_event(EventBusId id) {
    static constexpr auto button_list = {
      LocalNames::theme_button,
      LocalNames::asset_button,
      LocalNames::icon_button,
      LocalNames::font_button};
    auto model = ModelInScope();
    auto self = Generic(associated_object());
    for (const auto &button_name : button_list) {
      auto button = self.find<Generic>(button_name);
      if (model.instance.is_project_path_valid) {
        button.clear_state(State::disabled);
      } else {
        button.add_state(State::disabled);
      }
    }
    // set_project_button_enabled(e, model.instance.is_project_path_valid);
    if (model.instance.is_theme_updated) {
      Home::configure(screen().clean());
      model.instance.is_theme_updated = false;
    }
  }
};

auto &get_home_data() {
  static HomeData home_data(ViewObject::Names::home_object);
  return home_data;
}

} // namespace

Home::Home(const char *name) {
  construct_object(get_home_data().cast_as_name());
  fill();
  add(Row(ViewObject::Names::home_top_row)
        .fill()
        .set_padding(0)
        .add(Container(ViewObject::Names::home_container)
               .add_style("darker")
               .set_width(450)
               .fill_height()
               .set_border_side(BorderSide::right)
               .set_border_width(3)
               .add(Column().setup(configure_button_column)))
        .add(NakedContainer(LocalNames::content_area)
               .set_flex_grow()
               .fill_height()));

  auto configure_content = ModelInScope().instance.is_theme_updated
                             ? About::configure
                             : Project::configure;
  configure_content(screen().find<Generic>(LocalNames::content_area));

  if (auto model = ModelInScope(); model.instance.is_export_on_startup) {
    model.instance.runtime->push([](void *) {
      Event::send(
        screen().find(ViewObject::Names::project_object),
        EventCode::notified);
    });
  }
}
