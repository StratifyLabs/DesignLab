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
                .add(Column().fill_width().setup([](Column column) {
                  column.add(ScreenHeading("Theme Preview"));
                  configure_headings(column);
                  configure_buttons(column);
                  configure_badges(column);
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

  column.add(SectionHeading("Buttons"))
    .add(SectionHeading("Styles"))
    .add(Row().fill_width().setup([](Row row) {
      for (const auto *color : color_list) {
        row.add(Button()
                  .add_style(StringView("btn_") | color)
                  .add_static_label(color));
      }
    }))
    .add(Row().fill_width().setup([](Row row) {
      for (const auto *color : outline_color_list) {
        const auto label = var::KeyString(color).replace(
          KeyString::Replace().set_old_character('_').set_new_character(' '));
        row.add(
          Button().add_style(StringView("btn_") | color).add_label(label));
      }
    }));

  column.add(SectionHeading("Sizes"))
    .add(Row()
           .fill_width()
           .add(Button().add_style("btn_sm").add_label("Small"))
           .add(Button().add_style("btn_md").add_label("Medium"))
           .add(Button().add_style("btn_lg").add_label("Large")));
}

void ThemePreview::configure_badges(design::Column column) {
  column.add(SectionHeading("Badges"))
    .add(SectionHeading("Styles"))
    .add(Row().fill_width().setup([](Row row) {
      for (const auto *color : color_list) {
        row.add(
          Badge().add_style(StringView("bg_") | color).add_static_label(color));
      }
    }))
    .add(SectionHeading("Pills"))
    .add(Row().fill_width().setup([](Row row) {
      for (const auto *color : color_list) {
        const auto label = var::KeyString(color).replace(
          KeyString::Replace().set_old_character('_').set_new_character(' '));
        row.add(Badge()
                  .add_style(StringView("rounded_pill bg_") | color)
                  .add_label(label));
      }
    }));

  column.add(SectionHeading("Sizes"))
    .add(Row()
           .fill_width()
           .add(Badge().add_style("badge_sm").add_label("Small"))
           .add(Badge().add_style("badge_md").add_label("Medium"))
           .add(Badge().add_style("badge_lg").add_label("Large"))
           .add(Badge().add_style("badge_sm rounded_pill").add_label("Small"))
           .add(Badge().add_style("badge_md rounded_pill").add_label("Medium"))
           .add(Badge().add_style("badge_lg rounded_pill").add_label("Large")));

  column.add(SectionHeading("Examples"))
    .add(Row()
           .fill_width()
           .add(Button()
                  .add_style(Row::get_style())
                  .add_style("btn_lg")
                  .set_column_padding(10)
                  .add(Label().set_text_static("Inbox"))
                  .add(Badge()
                         .add_style("bg_danger rounded_pill badge_sm")
                         .add_static_label("99+"))));
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
