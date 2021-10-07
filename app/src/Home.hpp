#ifndef HOME_HPP
#define HOME_HPP

#include <lvgl.hpp>

#include "Application.hpp"
#include "FileBrowser.hpp"
#include "Elements.hpp"

class Home : public Application {
public:
  static void configure(lvgl::Container &container);

private:
  static constexpr auto tab_view_name = "homeTabView";
  static constexpr auto cloud_tab_name = "homeCloudTab";
  static constexpr auto application_tab_name = "homeApplicationTab";
  static constexpr auto os_tab_name = "homeOsTab";
  static constexpr auto filesystem_tab_name = "homeFileSystemTab";
  static constexpr auto about_tab_name = "aboutTab";
  static constexpr auto key_tab_name = "keyTab";
  static constexpr auto user_tab_name = "userTab";
  static constexpr auto team_tab_name = "teamTab";
  static constexpr auto task_tab_name = "taskTab";
  static constexpr auto debug_tab_name = "debugTab";
  static constexpr auto terminal_tab_name = "terminalTab";
  static constexpr auto exit_tab_name = "exitTab";

  struct Model {
    API_SINGLETON(Model);
  };

  static void configure_cloud(lvgl::Container &container) {

    static const auto user = var::KeyString(Icons::user).append(" User");
    static const auto users = var::KeyString(Icons::users).append(" Team");
    static const auto keys = var::KeyString(Icons::lock).append(" Keys");

    set_column_flow(container).add(E::Title("", "Cloud"))
      .add(Card().set_initial_icon(keys))
      .add(Card().set_initial_icon(user))
      .add(Card().set_initial_icon(users));
  }

  static void configure_application(lvgl::Container &container) {
    set_column_flow(container).add(Title("", "Application"));
  }

  static void configure_device(lvgl::Container &container) {
    set_column_flow(container).add(Title("", "Device"));



  }

  static void configure_filesystem(lvgl::Container &container) {
    set_column_flow(container)
      .add(Title("", "FileSystem"))
      .add(Container("").configure([](Container &object) {
        object.set_width(100_percent).set_height(90_percent);
        FileBrowser::configure(object);
      }));
  }


};

#endif // HOME_HPP
