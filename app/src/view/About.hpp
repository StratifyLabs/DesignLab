//
// Created by Tyler Gilbert on 12/29/21.
//

#ifndef DESIGNLAB_ABOUT_HPP
#define DESIGNLAB_ABOUT_HPP

#include "ViewObject.hpp"

class About : public ViewObject {
public:
  static void configure(Generic generic);

private:
  struct Names {
    DESIGN_DECLARE_NAME(about_column);
    DESIGN_DECLARE_NAME(dark_button);
    DESIGN_DECLARE_NAME(light_button);
  };

  static void update_theme(lv_event_t *e);
};

#endif // DESIGNLAB_ABOUT_HPP
