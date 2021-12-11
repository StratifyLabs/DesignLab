#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <sys/Cli.hpp>

#include "screens/ScreenObject.hpp"

using namespace lvgl;

class Application : public ScreenObject {

public:
  static void run(sys::Cli & cli);

  static Printer &printer() { return private_model().printer; }


protected:
  static constexpr auto tools_container_name = "ToolsGeneric";
  static constexpr auto canvas_container_name = "CanvasGeneric";
  static constexpr auto right_tab_view_name = "RightTabView";
  static constexpr auto properties_container_name = "PropertiesGeneric";
  static constexpr auto tree_container_name = "TreeGeneric";


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
    Event::send(screen().find(properties_container_name), EventCode::notified, selected);
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

  static void configure_splash(lvgl::Generic &container);
  static void configure_login(lvgl::Generic &container);
};

#endif // APPLICATION_HPP
