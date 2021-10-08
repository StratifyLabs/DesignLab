#ifndef PROPERTIES_HPP
#define PROPERTIES_HPP

#include <var/StackString.hpp>

#include "Application.hpp"

class Properties : public Application {
public:
  static void configure(Container &container) {
    static constexpr auto add_button_name = "AddButton";
    static constexpr auto add_list_name = "AddList";

    static auto screen = Container::active_screen();

    static auto set_property = [&](Property property, lv_coord_t value) {
      screen.find(Style::to_cstring(property))
        .find(E::Property::text_area_name)
        .cast<TextArea>()
        ->set_text(var::NumberString(value));
    };

    static const auto width_context = E::Property::Context(Property::width);
    static const auto height_context = E::Property::Context(Property::height);
    container
      .add_event_callback(
        EventCode::notified,
        [](lv_event_t *e) {
          const Event event(e);

          const auto prop = Property::width;
          Container object(event.parameter<lv_obj_t*>());

          set_property(Property::width, object.get_property_value(Property::width).number());
          set_property(Property::height, object.get_property_value(Property::height).number());

        })
      .add(E::Title("", "Properties"))
      .add(E::Spacer())
      .add(E::PropertySectionTitle("Position"))
      .add(E::Property(width_context))
      .add(E::Property(height_context));
  }
};

#endif // PROPERTIES_HPP
