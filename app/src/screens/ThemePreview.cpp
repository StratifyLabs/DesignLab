//
// Created by Tyler Gilbert on 12/4/21.
//

#include <design.hpp>
#include <lvgl.hpp>

#include "ThemePreview.hpp"

void ThemePreview::configure(lvgl::Generic generic) {
  generic.add(Container().fill().add(Column().fill().setup([](Column column) {
    column.add(ScreenHeading("Theme Preview"));
    configure_headings(column);
    configure_buttons(column);
    configure_cards(column);
  })));
}

void ThemePreview::configure_headings(design::Column column) {
  column.add(SectionHeading("Typography").fill_width())
    .add(SubSectionHeading("Headings").fill_width())
    .add(Heading1("h1. Heading 1").fill_width())
    .add(Heading2("h2. Heading 2").fill_width())
    .add(Heading3("h3. Heading 3").fill_width())
    .add(Heading4("h4. Heading 4").fill_width());
}

void ThemePreview::configure_buttons(design::Column column) {

  column.add(SectionHeading("Button Styles"))
    .add(Row()
           .fill_width()
           .add(Button().add_style("primary").add_label("Primary"))
           .add(Button().add_style("secondary").add_label("Secondary"))
           .add(Button().add_style("info").add_label("Info"))
           .add(Button().add_style("warning").add_label("Warning"))
           .add(Button().add_style("danger").add_label("Danger"))
           .add(Button().add_style("success").add_label("Success")))
    .add(
      Row()
        .fill_width()
        .add(Button().add_style("outline_primary").add_label("Outline Primary"))
        .add(Button()
               .add_style("outline_secondary")
               .add_label("Outline Secondary"))
        .add(Button().add_style("outline_info").add_label("Outline Info"))
        .add(Button().add_style("outline_warning").add_label("Outline Warning"))
        .add(Button().add_style("outline_danger").add_label("Outline Danger"))
        .add(
          Button().add_style("outline_success").add_label("Outline Success")));

  column.add(SectionHeading("Button Sizes"))
    .add(Row()
           .fill_width()
           .add(Button().add_style("sm").add_label("Small"))
           .add(Button().add_label("Medium"))
           .add(Button().add_style("lg").add_label("Large")));
}

void ThemePreview::configure_cards(design::Column column) {
  column.add(SectionHeading("Cards")).add(SubSectionHeading("Card Styles"));

  static auto populate_card = [](Card card) {
    card.set_width(45_percent)
      .add_style(Column::get_style())
      .add(Card::Header("Card Header"))
      .add(Card::Body("This is some text within a card body."))
      .add(Card::Footer("Card Footer"));
  };

  column.add(Row().fill_width().add(
    Card("Default").set_width(50_percent).setup([](Card card) {
      card.add(Card::Body("This is some text within a card body."));
    })));

  column.add(
    Row()
      .fill_width()
      .add(Card("Default").setup([](Card card) { populate_card(card); }))
      .add(Card("Primary").add_style("primary").setup([](Card card) { populate_card(card); })));
}
