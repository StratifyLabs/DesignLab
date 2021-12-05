//
// Created by Tyler Gilbert on 12/4/21.
//

#include <design.hpp>
#include <lvgl.hpp>

#include "ThemePreview.hpp"

void ThemePreview::configure(lvgl::Container container) {
  container.set_padding(20).add(Column().fill().set_row_padding(20).setup(
    [](Column column) {
      configure_headings(column);
      configure_buttons(column);
    }));
}

void ThemePreview::configure_headings(design::Column column) {
  column.add(Row().fill_width().add(Heading1("", "h1. Heading 1")))
    .add(Row().fill_width().add(Heading2("", "h2. Heading 2")))
    .add(Row().fill_width().add(Heading3("", "h3. Heading 3")))
    .add(Row().fill_width().add(Heading4("", "h4. Heading 4")));
}

void ThemePreview::configure_buttons(design::Column column) {

  column.add(Row().fill_width().add(Heading1("", "Button Styles")))
    .add(Row()
           .fill_width()
           .set_column_padding(10)
           .add(Button().add_style("primary").add_label("Primary"))
           .add(Button().add_style("secondary").add_label("Secondary"))
           .add(Button().add_style("info").add_label("Info"))
           .add(Button().add_style("warning").add_label("Warning"))
           .add(Button().add_style("danger").add_label("Danger"))
           .add(Button().add_style("success").add_label("Success")))
    .add(
      Row()
        .fill_width()
        .set_column_padding(10)
        .add(Button().add_style("outline_primary").add_label("Outline Primary"))
        .add(Button()
               .add_style("outline_secondary")
               .add_label("Outline Secondary"))
        .add(Button().add_style("outline_info").add_label("Outline Info"))
        .add(Button().add_style("outline_warning").add_label("Outline Warning"))
        .add(Button().add_style("outline_danger").add_label("Outline Danger"))
        .add(
          Button().add_style("outline_success").add_label("Outline Success")));

  column.add(Row().fill_width().add(Label().set_text_static("Button Sizes")))
    .add(Row()
           .fill_width()
           .set_column_padding(10)
           .add(Button().add_style("sm").add_label("Small"))
           .add(Button().add_label("Medium"))
           .add(Button().add_style("lg").add_label("Large")));
}