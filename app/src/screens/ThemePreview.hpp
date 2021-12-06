//
// Created by Tyler Gilbert on 12/4/21.
//

#ifndef DESIGNLAB_THEMEPREVIEW_HPP
#define DESIGNLAB_THEMEPREVIEW_HPP

#include "GuiObject.hpp"

class ThemePreview : public GuiObject {
public:
  static constexpr auto screen_name = "ThemePreview";
  static void configure(lvgl::Generic container);

private:
  static void configure_headings(design::Column column);
  static void configure_buttons(design::Column column);
  static void configure_cards(design::Column column);

};

#endif // DESIGNLAB_THEMEPREVIEW_HPP
