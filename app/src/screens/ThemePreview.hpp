//
// Created by Tyler Gilbert on 12/4/21.
//

#ifndef DESIGNLAB_THEMEPREVIEW_HPP
#define DESIGNLAB_THEMEPREVIEW_HPP

#include "ScreenObject.hpp"

class ThemePreview : public ScreenObject {
public:
  static constexpr auto screen_name = "ThemePreview";
  static void configure(lvgl::Generic container);

private:
  static constexpr auto color_list = {
    "default",
    "primary",
    "secondary",
    "info",
    "warning",
    "danger",
    "success",
    "light",
    "dark"};
  static constexpr auto outline_color_list = {
    "outline_default",
    "outline_primary",
    "outline_secondary",
    "outline_info",
    "outline_warning",
    "outline_danger",
    "outline_success",
    "outline_light",
    "outline_dark"};

  static void configure_headings(design::Column column);
  static void configure_buttons(design::Column column);
  static void configure_badges(design::Column column);
  static void configure_cards(design::Column column);
  static void configure_checklists(design::Column column);
};

#endif // DESIGNLAB_THEMEPREVIEW_HPP
