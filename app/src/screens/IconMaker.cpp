//
// Created by Tyler Gilbert on 12/12/21.
//

#include <lvgl.hpp>

#include "IconMaker.hpp"

void IconMaker::configure(Generic generic) {

  generic.add(Container().fill().add(Column().fill().setup([](Column column) {
    column.add(ScreenHeading("Icons")).add(HorizontalLine());

    column.add(SectionHeading("Bootstrap Icons"));
    column.add(SectionHeading("Font Awesome Icons"));

    column.add(Checkbox("").set_text_static("Icon Name"));
   // column.add(Form().setup(configure_form_input));
  })));
}