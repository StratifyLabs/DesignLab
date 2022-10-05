//
// Created by Tyler Gilbert on 12/4/21.
//

#ifndef DESIGNLAB_THEMEPREVIEW_HPP
#define DESIGNLAB_THEMEPREVIEW_HPP

#include "ViewObject.hpp"

class ThemePreview : public ViewObject {
public:
  static constexpr auto screen_name = "ThemePreview";
  static void configure(lvgl::Generic container);
};

#endif // DESIGNLAB_THEMEPREVIEW_HPP
