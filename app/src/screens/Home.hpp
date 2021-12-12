//
// Created by Tyler Gilbert on 12/8/21.
//

#ifndef DESIGNLAB_HOME_HPP
#define DESIGNLAB_HOME_HPP

#include "ScreenObject.hpp"

class Home : public ScreenObject {
public:
  static void configure(Generic generic);

private:
  struct Names {
    static constexpr auto button_area = "HomeButtonArea";
    static constexpr auto content_area = "HomeContentArea";
  };

  static void add_side_button(
    Column column,
    const char *icon,
    const char *name,
    void (*callback)(Generic));

  static void configure_button_column(Column column);
};

#endif // DESIGNLAB_HOME_HPP
