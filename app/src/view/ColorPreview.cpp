//
// Created by Tyler Gilbert on 12/19/21.
//

#include <lvgl.hpp>

#include "ColorPreview.hpp"

void ColorPreview::configure(Generic generic) {

  generic.add(Column().set_row_padding(20).fill().add(
    ColorWheel().set_arc_width(64).set_width(80_percent).set_height(80_percent)));

}
