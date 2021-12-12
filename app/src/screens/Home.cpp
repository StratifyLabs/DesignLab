//
// Created by Tyler Gilbert on 12/8/21.
//

#include "ThemeMaker.hpp"
#include "ThemePreview.hpp"

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
                    .fill()
                    .add(SectionHeading("Design Lab"))
                    .setup(configure_button_column)))
      .add(NakedContainer(content_area).set_flex_grow().fill_height()));
}

void Home::configure_button_column(Column column) {

  add_side_button(column, "", "Theme Maker", ThemeMaker::configure);
  add_side_button(column, "", "Theme Preview", ThemePreview::configure);
  add_side_button(column, "", "Icons", nullptr);

  column.add(NakedContainer().set_flex_grow())
    .add(Label().set_text_static("Account"));
}

void Home::add_side_button(
  Column column,
  const char *icon,
  const char *name,
  void (*callback)(Generic)) {
  column.add(
    Button(name).fill_width().add_static_label(name).add_event_callback(
      EventCode::clicked,
      (void *)callback,
      [](lv_event_t *e) {
        const auto callback = Event(e).user_data<void (*)(Generic)>();
        if (callback) {
          callback(screen().find<Generic>(content_area).clean());
        }
      }));
};
