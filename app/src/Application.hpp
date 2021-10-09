#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <api/api.hpp>
#include <chrono/ClockTime.hpp>
#include <json/Json.hpp>
#include <lvgl/Container.hpp>
#include <lvgl/Event.hpp>
#include <lvgl/Group.hpp>
#include <lvgl/PeriodicTimer.hpp>
#include <printer.hpp>
#include <var/Queue.hpp>
#include <var/StackString.hpp>
#include <lvgl/Button.hpp>
#include <lvgl/Container.hpp>
#include <lvgl/ObjectAccess.hpp>
#include <lvgl/TextArea.hpp>
#include <lvgl/List.hpp>
#include <lvgl/Group.hpp>
#include <lvgl/Event.hpp>

using namespace lvgl;

class Application {

public:
  Application(lvgl::Group keyboard_group);

  static Printer &printer() { return private_model().printer; }

  struct Model {
  public:
    API_SINGLETON(Model);
    lvgl::Font title_font;
    Style column_flow_style;
    Style fill_parent_style;
    var::Queue<chrono::ClockTime::UniqueString> name_list;
    lv_obj_t * selected_object = nullptr;
    var::Queue<var::Vector<FormList::ItemUserData>> tree;
  };

  static Model &model() { return Model::instance(); }

protected:
  static constexpr auto button_height = 10_percent;

  static constexpr auto tools_container_name = "ToolsContainer";
  static constexpr auto canvas_container_name = "CanvasContainer";
  static constexpr auto right_tab_view_name = "RightTabView";
  static constexpr auto properties_container_name = "PropertiesContainer";
  static constexpr auto tree_container_name = "TreeContainer";

  static lvgl::Container &set_column_flow(lvgl::Container &container) {
    return container.set_flex_layout()
      .set_flex_flow(FlexFlow::column)
      .set_column_padding(10)
      .set_flex_align(SetFlexAlign().set_main(FlexAlign::start));
  }

  static const char * generate_name() {
    return model()
      .name_list.push(chrono::ClockTime::get_system_time().to_unique_string())
      .back().cstring();
  }

  static const char * latest_name() {
    return model().name_list.back().cstring();
  }

  static void notify_properties(lv_obj_t * selected){
    model().selected_object = selected;
    Event::send(Container::active_screen().find(properties_container_name), EventCode::notified, selected);
  }

private:
  struct PrivateModel {
  public:
    API_SINGLETON(PrivateModel);
    printer::Printer printer;
    lvgl::Font title_font;
    lvgl::Group keyboard_group;
    lvgl::PeriodicTimer periodic_timer;
  };

  static PrivateModel &private_model() { return PrivateModel::instance(); }

  static constexpr auto application_tile_view_name = "ApplicationTileView";
  static constexpr auto application_splash_tile_name = "ApplicationSplashTile";
  static constexpr auto application_login_tile_name = "ApplicationLoginTile";
  static constexpr auto application_home_tile_name = "ApplicationHomeTile";

  static void handle_periodic(lv_timer_t *);

  static void configure_splash(lvgl::Container &container);
  static void configure_login(lvgl::Container &container);
};

#endif // APPLICATION_HPP
