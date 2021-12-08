//
// Created by Tyler Gilbert on 12/4/21.
//

#include <design.hpp>
#include <lvgl.hpp>
#include <var.hpp>

#include "ThemePreview.hpp"

void ThemePreview::configure(lvgl::Generic generic) {
  generic.add(Container()
                .set_scroll_mode(ScrollBarMode::auto_)
                .fill()
                .add(Column()
                       .fill_width()
                       .setup([](Column column) {
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

#if 1
  column.add(SectionHeading("Button Styles"))
    .add(Row().fill_width().setup([](Row row) {
      for (const auto *color : color_list) {
        row.add(Button().add_style(color).add_static_label(color));
      }
    }))
    .add(Row().fill_width().setup([](Row row) {
      for (const auto *color : outline_color_list) {
        const auto label = var::KeyString(color).replace(
          KeyString::Replace().set_old_character('_').set_new_character(' '));
        row.add(Button().add_style(color).add_label(label));
      }
    }));
#endif

  column.add(SectionHeading("Button Sizes"))
    .add(Row()
           .fill_width()
           .add(Button().add_style("sm").add_label("Small"))
           .add(Button().add_style("md").add_label("Medium"))
           .add(Button().add_style("lg").add_label("Large")));
}

void ThemePreview::configure_cards(design::Column column) {
  column.add(SectionHeading("Cards")).add(SubSectionHeading("Card Styles"));

  static auto populate_card = [](Card card, const char *color) {
    card.set_width(15_percent)
      .add_style(Column::get_style())
      .add(Card::Header("Card Header"))
      .add(Card::Body("This is some text within a card body."))
      .add(Card::Footer("Card Footer"));
  };

  column.add(Row().fill_width().add(
    Card("Default").set_width(50_percent).setup([](Card card) {
      card.add(Card::Body("This is some text within a card body."));
    })));

  column.add(Row().fill_width().setup([](Row row) {
    for (const auto *color : color_list) {
      row.add(Card(color).add_style(color).setup(
        [](Card card) { populate_card(card, card.name()); }));
    }
  }));
}
