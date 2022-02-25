//
// Created by Tyler Gilbert on 12/19/21.
//

#ifndef DESIGNLAB_COLORPREVIEW_HPP
#define DESIGNLAB_COLORPREVIEW_HPP


#include "ViewObject.hpp"

class ColorPreview : public ViewObject, public ObjectAccess<ColorPreview> {
public:
  static void configure(Generic generic){
    generic.add(ColorPreview(ViewObject::Names::color_preview_object));
  }

private:
  LVGL_OBJECT_ACCESS_DECLARE_CONSTRUCTOR(ColorPreview);
  struct Names {
    DESIGN_DECLARE_NAME(rgb_splotch_container);
    DESIGN_DECLARE_NAME(rgb_splotch_button);
    DESIGN_DECLARE_NAME(rgb_splotch_label);
    DESIGN_DECLARE_NAME(rgb_color_control_row);
    DESIGN_DECLARE_NAME(shade_color_row);
    DESIGN_DECLARE_NAME(tint_color_row);
    DESIGN_DECLARE_NAME(rgb_control_column);
    DESIGN_DECLARE_NAME(Red);
    DESIGN_DECLARE_NAME(Green);
    DESIGN_DECLARE_NAME(Blue);
  };

  static constexpr size_t tint_shade_count = 10;

  static void rgb_color_changed(lv_event_t *e);

  static void update_tint_colors(Row row, Color color);
  static void update_shade_colors(Row row, Color color);

  static void color_clicked(lv_event_t*e);
};

#endif // DESIGNLAB_COLORPREVIEW_HPP
