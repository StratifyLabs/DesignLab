#ifndef PROPERTIES_HPP
#define PROPERTIES_HPP

#include <var/StackString.hpp>

#include "Elements.hpp"

class Properties : public Application {
public:
  static void configure(Container &container) {
    static auto screen = Container::active_screen();

    static auto set_property = [&](Property property, lv_coord_t value) {
      screen.find(Style::to_cstring(property))
        .find<TextArea>("textAreaName")
        .set_text(var::NumberString(value));
    };

    // static const auto width_context = E::Property::UserData(Property::width);
    // static const auto height_context =
    // E::Property::UserData(Property::height);

    container
      .add_event_callback(
        EventCode::notified,
        [](lv_event_t *e) {
          const Event event(e);
          Container object(event.parameter<lv_obj_t *>());

          set_property(
            Property::width,
            object.get_property_value(Property::width).number());
          set_property(
            Property::height,
            object.get_property_value(Property::height).number());
        });
#if 0
      .add(E::Title("", "Properties"))
      .add(E::Spacer())
      .add(E::PropertySectionTitle("Position"))
      .add(E::Property(width_context))
      .add(E::Property(height_context));
#endif
  }
};

#endif // PROPERTIES_HPP
