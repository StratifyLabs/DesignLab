#ifndef SPACER_HPP
#define SPACER_HPP

#include <lvgl/Label.hpp>
#include <lvgl/ObjectAccess.hpp>

namespace lvgl {

OBJECT_ACCESS_FORWARD_FRIENDS();

class Spacer : public lvgl::ObjectAccess<Spacer> {
public:
  explicit Spacer() : ObjectAccess("") {}

private:
  OBJECT_ACCESS_FRIENDS();
  Spacer(Object parent, const Spacer &options) {
    m_object = api()->label_create(parent.object());
    api()->label_set_text_static(m_object, "");
    api()->obj_set_width(m_object, LV_PCT(100));
    api()->obj_set_height(m_object, options.initial_height());
  }

  API_AF(Spacer, lv_coord_t, initial_height, 0);
};

class Title : public lvgl::ObjectAccess<Title> {
public:
  explicit Title(const char *name, const char *title)
    : ObjectAccess(name), m_initial_title(title) {}

  static constexpr lv_coord_t height = 120;

private:
  OBJECT_ACCESS_FRIENDS();
  Title(Object parent, const Title &options) {
    m_object = api()->label_create(parent.object());
    cast<Label>()->set_text_static(options.m_initial_title)
      .set_text_font(Font::find(80, Font::Style::semi_bold))
      .set_width(100_percent)
      .set_height(height);
  }

  const char *m_initial_title;
};

} // namespace lvgl

#endif // SPACER_HPP
