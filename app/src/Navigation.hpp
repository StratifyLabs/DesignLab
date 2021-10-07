#ifndef NAVIGATION_HPP
#define NAVIGATION_HPP

#include <lvgl/Button.hpp>
#include <lvgl/Container.hpp>
#include <lvgl/ObjectAccess.hpp>

#include "Icons.hpp"

namespace lvgl {

OBJECT_ACCESS_FORWARD_FRIENDS();

class Navigation : public lvgl::ObjectAccess<Navigation> {
public:
  explicit Navigation() : ObjectAccess("") {}

private:
  OBJECT_ACCESS_FRIENDS();
  Navigation(Object parent, const Navigation &) {
    m_object = api()->obj_create(parent.object());

    Container container(m_object);
    container
      .set_vertical_padding(0)
      .set_width(95_percent)
      .set_alignment(Alignment::center)
      .set_height(10_percent)
      .add(Button(Icons::chevron_left).configure([](Button &button) {
        button.set_x(2_percent)
          .set_width(15_percent)
          .set_height(95_percent)
          .add_flag(Flags::event_bubble)
          .add_label();
      }))
      .add(Button(Icons::chevron_right).configure([](Button &button) {
        button.set_x(83_percent)
          .set_width(15_percent)
          .set_height(95_percent)
          .add_flag(Flags::event_bubble)
          .add_label();
      }));
  }
};

} // namespace lvgl

#endif // NAVIGATION_HPP
