//
// Created by Tyler Gilbert on 12/19/21.
//

#ifndef DESIGNLAB_COLORPREVIEW_HPP
#define DESIGNLAB_COLORPREVIEW_HPP


#include "ViewObject.hpp"

class ColorPreview : public ViewObject {
public:
  static void configure(Generic generic);

private:
  struct Names {
    DESIGN_DECLARE_NAME(rgb_splotch_container);
    DESIGN_DECLARE_NAME(hsv_splotch_container);
    DESIGN_DECLARE_NAME(rgb_color_control_row);
    DESIGN_DECLARE_NAME(hsv_color_control_row);
    DESIGN_DECLARE_NAME(rgb_control_column);
    DESIGN_DECLARE_NAME(hsv_control_column);
    DESIGN_DECLARE_NAME(Red);
    DESIGN_DECLARE_NAME(Green);
    DESIGN_DECLARE_NAME(Blue);
    DESIGN_DECLARE_NAME(Hue);
    DESIGN_DECLARE_NAME(Saturation);
    DESIGN_DECLARE_NAME(Value);
  };

  static void rgb_color_changed(lv_event_t *e);
  static void hsv_color_changed(lv_event_t *e);
};

#endif // DESIGNLAB_COLORPREVIEW_HPP
