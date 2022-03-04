//
// Created by Tyler Gilbert on 12/19/21.
//

#include <lvgl.hpp>
#include <var.hpp>
#include <window/Clipboard.hpp>

#include <design/extras/NotificationToast.hpp>

#include "ColorPreview.hpp"
#include "extras/Extras.hpp"

ColorPreview::ColorPreview(const char *name) {
  construct_object(name);
  fill();

  add(Column(ViewObject::Names::content_container)
        .add_style("container")
        .fill()
        .add(ScreenHeading("Color Picker"))
        .add(HorizontalLine())
        .add(SectionHeading("RGB Color Selector"))
        .add(Row(Names::rgb_color_control_row).fill_width())
        .add(SubSectionHeading("Tints"))
        .add(Row(Names::tint_color_row).fill_width())
        .add(SubSectionHeading("Shades"))
        .add(Row(Names::shade_color_row).fill_width()));

  auto rgb_row = find<Row>(Names::rgb_color_control_row);

  auto configure_tint_shade_row = [](Row row) {
    row.set_flex_flow(FlexFlow::row_wrap)
      .set_row_padding(0)
      .set_column_padding(0);
  };

  rgb_row.justify_space_evenly()
    .add(Column(Names::rgb_control_column)
           .justify_space_between()
           .set_height(500)
           .set_width(30_percent)
           .add(ColorSlider(Names::Red, 255, rgb_color_changed).fill_width())
           .add(ColorSlider(Names::Green, 255, rgb_color_changed).fill_width())
           .add(ColorSlider(Names::Blue, 255, rgb_color_changed).fill_width()))
    .add(
      Container(Names::rgb_splotch_container)
        .add(
          Button(Names::rgb_splotch_button)
            .center()
            .add_event_callback(EventCode::clicked, color_clicked)
            .add(Label(Names::rgb_splotch_label).set_text("0x000000").center()))
        .set_background_opacity(Opacity::opaque)
        .set_background_color(Color::black())
        .set_radius(Container::radius_circle)
        .set_height(450)
        .set_width(450));

  auto tint_row = find<Row>(Names::tint_color_row);
  configure_tint_shade_row(tint_row);
  for (auto i : api::Index(tint_shade_count)) {
    tint_row.add(ColorButton("", Color::grey(), color_clicked));
  }

  update_tint_colors(tint_row, Color::grey());

  auto shade_row = find<Row>(Names::shade_color_row);
  configure_tint_shade_row(shade_row);
  for (auto i : api::Index(tint_shade_count)) {
    shade_row.add(ColorButton("", Color::grey(), color_clicked));
  }

  update_shade_colors(shade_row, Color::grey());
}

void ColorPreview::rgb_color_changed(lv_event_t *e) {
  auto parent
    = Event(e).find_parent<Container>(ViewObject::Names::content_container);
  const StringView name = Event(e).current_target().name();

  const auto red = parent.find<ColorSlider>(Names::Red).get_u8();
  const auto green = parent.find<ColorSlider>(Names::Green).get_u8();
  const auto blue = parent.find<ColorSlider>(Names::Blue).get_u8();
  const auto updated_color = Color(Color::Rgb{red, green, blue});

  parent.find<Container>(Names::rgb_splotch_container)
    .set_background_color(updated_color);

  parent.find<Label>(Names::rgb_splotch_label)
    .set_text(
      NumberString(updated_color.get_color().full & 0xffffff, "0x%06x"));

  update_tint_colors(parent.find<Row>(Names::tint_color_row), updated_color);
  update_shade_colors(parent.find<Row>(Names::shade_color_row), updated_color);
}

void ColorPreview::update_tint_colors(Row row, Color color) {

  u8 mix_ratio = 0;
  for (auto object : row) {
    auto color_button = object.get<ColorButton>();
    color_button.set_color(Color(color).lighten(MixRatio(mix_ratio)));
    mix_ratio += 255 / tint_shade_count;
  }
}

void ColorPreview::update_shade_colors(Row row, Color color) {
  u8 mix_ratio = 0;
  for (auto object : row) {
    auto color_button = object.get<ColorButton>();
    color_button.set_color(Color(color).darken(MixRatio(mix_ratio)));
    mix_ratio += 255 / tint_shade_count;
  }
}

void ColorPreview::color_clicked(lv_event_t *e) {
  const char *text = Event(e).target().get_child(0).get<Label>().get_text();
  window::Clipboard::set_text(text);
  NotificationToast(2_seconds)
    .add_style("toast_bottom_right")
    .add_style("bg_success text_success")
    .set_title(ICONS_FA_CLONE_SOLID " Copied")
    .set_message("Copied " | StringView(text) | " to clipboard");
}
