//
// Created by Tyler Gilbert on 12/29/21.
//

#include "extras/Extras.hpp"
#include "About.hpp"


void About::configure(Generic generic) {

  generic.clear_flag(Flags::scrollable)
    .add(Container(ViewObject::Names::content_container)
           .fill()
           .add(Column(Names::about_column)));

  auto column = generic.find<Column>(Names::about_column);
  column.fill()
    .add(HeaderRow(
      "About",
      icons::fa::info_circle_solid,
      "Check for Updates",
      nullptr))
    .add(HorizontalLine());
}
