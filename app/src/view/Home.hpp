//
// Created by Tyler Gilbert on 12/8/21.
//

#ifndef DESIGNLAB_HOME_HPP
#define DESIGNLAB_HOME_HPP

#include "ViewObject.hpp"

class Home : public ViewObject, public ObjectAccess<Home> {
public:
  static void configure(Generic generic){
    generic.add(Home(ViewObject::Names::home_object));
  }

private:
  LVGL_OBJECT_ACCESS_DECLARE_CONSTRUCTOR(Home);
  struct Names {
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

  static void add_side_button(
    Column column,
    const char *icon,
    const char *name,
    void (*callback)(Generic));

  static void configure_button_column(Column column);
  static void update_buttons(lv_event_t*);
  static void notified(lv_event_t*);

  static void set_project_button_enabled(lv_event_t * e, bool value = true);

};

#endif // DESIGNLAB_HOME_HPP
