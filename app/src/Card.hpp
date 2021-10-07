#ifndef CARD_HPP
#define CARD_HPP

#include <lvgl/Button.hpp>
#include <lvgl/Container.hpp>
#include <lvgl/ObjectAccess.hpp>

#include "Icons.hpp"

namespace lvgl {

OBJECT_ACCESS_FORWARD_FRIENDS();

class Card : public lvgl::ObjectAccess<Card> {
public:
  explicit Card(const char *name = "") : ObjectAccess(name) {}

private:
  OBJECT_ACCESS_FRIENDS();
  Card(Object parent, const Card &options) {
    m_object = api()->btn_create(parent.object());

    Container container(m_object);

    static constexpr lv_coord_t height = 150;
    static constexpr lv_coord_t icon_height = 72;
    static constexpr lv_coord_t icon_offset = (height - icon_height) / 2;
    static const auto icon_font = Font::find("montserrat", icon_height);

    container.set_vertical_padding(0)
      .set_horizontal_padding(20)
      .set_width(95_percent)
      .set_alignment(Alignment::center)
      .set_height(height)
      .add(Label("label").configure([](Label &object) {
        object.set_y(icon_offset)
          .set_text_font(icon_font)
          .set_width(90_percent)
          .set_height(95_percent);
      }))
      .add(Label(Icons::chevron_right).configure([](Label &object) {
        object.set_y(icon_offset)
          .set_text_font(icon_font)
          .set_x(90_percent)
          .set_width(10_percent)
          .set_text_alignment(TextAlignment::right)
          .set_height(icon_height)
          .set_text_static(Icons::chevron_right);
      }));

    container.find("label").cast<Label>()->set_text(options.initial_icon());
  }

  API_AF(Card, const char *, initial_icon, "");
};

} // namespace lvgl

#endif // NAVIGATION_HPP
