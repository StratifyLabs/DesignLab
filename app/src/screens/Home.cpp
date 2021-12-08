//
// Created by Tyler Gilbert on 12/8/21.
//

#include "Home.hpp"

void Home::configure(Generic generic) {

  generic.add(
    Row()
      .fill()
      .set_padding(0)
      .add(Container()
             .add_style("darker")
             .set_width(450)
             .fill_height()
             .set_border_side(BorderSide::right)
             .set_border_width(3)
             .add(Column()
                    .add_style("darker")
                    .fill_height()
                    .fill_width()
                    .add(SectionHeading("Design Lab"))
                    .add(Button().fill_width().add_static_label("Theme Maker"))
                    .add(NakedContainer().set_flex_grow())
                    .add(Label().set_text_static("Account"))))
      .add(Column()
             .set_flex_grow()
             .set_padding(0)
             .set_border_side(BorderSide::right)
             .set_border_width(3)));

}
