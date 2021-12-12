//
// Created by Tyler Gilbert on 12/8/21.
//

#include "ThemeMaker.hpp"

void ThemeMaker::configure(lvgl::Generic generic) {

  generic.add(Container().fill().add(Column().fill().setup([](Column column) {
    printer().info("Add Theme Maker Header");
    column.add(ScreenHeading("Theme Maker")).add(HorizontalLine());
  })));
}
