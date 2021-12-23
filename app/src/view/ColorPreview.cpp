//
// Created by Tyler Gilbert on 12/19/21.
//

#include <lvgl.hpp>
#include <var.hpp>

#include "ColorPreview.hpp"
#include "extras/Extras.hpp"

void ColorPreview::configure(Generic generic) {

  generic.add(Column(ViewObject::Names::content_container)
                .add_style("container")
                .fill()
                .add(ScreenHeading("Color Picker"))
                .add(HorizontalLine())
                .add(SectionHeading("RGB Color Selector"))
                .add(Row(Names::rgb_color_control_row).fill_width())
                .add(HorizontalLine())
                .add(SectionHeading("HSV Color Selector"))
                .add(Row(Names::hsv_color_control_row).fill_width()));

  auto rgb_row = generic.find<Row>(Names::rgb_color_control_row);

  rgb_row.justify_space_evenly()
    .add(Column(Names::rgb_control_column)
           .justify_space_between()
           .set_height(500)
           .set_width(30_percent)
           .add(ColorSlider(Names::Red, 255, rgb_color_changed).fill_width())
           .add(ColorSlider(Names::Green, 255, rgb_color_changed).fill_width())
           .add(ColorSlider(Names::Blue, 255, rgb_color_changed).fill_width()))
    .add(Container(Names::rgb_splotch_container)
           .set_background_opacity(Opacity::opaque)
           .set_background_color(Color::black())
           .set_radius(Container::radius_circle)
           .set_height(450)
           .set_width(450));

  auto hsv_row = generic.find<Row>(Names::hsv_color_control_row);

  hsv_row.justify_space_evenly()
    .add(Container(Names::hsv_splotch_container)
           .set_background_opacity(Opacity::opaque)
           .set_background_color(Color::black())
           .set_radius(Container::radius_circle)
           .set_height(450)
           .set_width(450))
    .add(
      Column(Names::hsv_control_column)
        .justify_space_between()
        .set_height(500)
        .set_width(30_percent)
        .add(ColorSlider(Names::Hue, 360, hsv_color_changed).fill_width())
        .add(
          ColorSlider(Names::Saturation, 100, hsv_color_changed).fill_width())
        .add(ColorSlider(Names::Value, 100, hsv_color_changed).fill_width()));
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
}

void ColorPreview::hsv_color_changed(lv_event_t *e) {
  auto parent
    = Event(e).find_parent<Container>(ViewObject::Names::content_container);
  const StringView name = Event(e).current_target().name();

  const auto hue = parent.find<ColorSlider>(Names::Hue).get_u16();
  const auto saturation = parent.find<ColorSlider>(Names::Saturation).get_u8();
  const auto value = parent.find<ColorSlider>(Names::Value).get_u8();
  const auto updated_color = Color(Color::Hsv{hue, saturation, value});

  parent.find<Container>(Names::hsv_splotch_container)
    .set_background_color(updated_color);
}
