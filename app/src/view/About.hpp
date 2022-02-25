//
// Created by Tyler Gilbert on 12/29/21.
//

#ifndef DESIGNLAB_ABOUT_HPP
#define DESIGNLAB_ABOUT_HPP

#include "ViewObject.hpp"

class About : public ViewObject, public ObjectAccess<About> {
public:
  static void configure(Generic generic){
    generic.add(About(ViewObject::Names::about_object));
  }

private:
  LVGL_OBJECT_ACCESS_DECLARE_CONSTRUCTOR(About);

  struct Names {
    DESIGN_DECLARE_NAME(about_column);
    DESIGN_DECLARE_NAME(dark_button);
    DESIGN_DECLARE_NAME(light_button);
  };

  static void update_theme(lv_event_t *e);
};

#endif // DESIGNLAB_ABOUT_HPP
