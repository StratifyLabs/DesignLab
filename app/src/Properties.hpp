#ifndef PROPERTIES_HPP
#define PROPERTIES_HPP

#include <var/StackString.hpp>

#include "Application.hpp"

class Properties : public Application {
public:
  static void configure(Container &container) {
    static constexpr auto add_button_name = "AddButton";
    static constexpr auto add_list_name = "AddList";

    static constexpr auto x_name = "xProp";
    static constexpr auto y_name = "yProp";
    static constexpr auto width_name = "widthProp";
    static constexpr auto height_name = "heightProp";
    static constexpr auto content_width_name = "contentWidthProp";
    static constexpr auto content_height_name = "contentHeightProp";

    static constexpr auto border_left_name = "borderLeftProp";
    static constexpr auto border_right_name = "borderRightProp";
    static constexpr auto border_top_name = "borderTopProp";
    static constexpr auto border_bottom_name = "borderBottomProp";

    static constexpr auto outline_padding = "outlinePaddingProp";

    static constexpr auto padding_left_name = "paddingLeftProp";
    static constexpr auto padding_right_name = "paddingRightProp";
    static constexpr auto padding_top_name = "paddingTopProp";
    static constexpr auto padding_bottom_name = "paddingBottomProp";

    static auto screen = Container::active_screen();

    static auto set_property = [&](const char *name, lv_coord_t value) {
      screen.find(name)
        .find(E::Property::text_area_name)
        .cast<TextArea>()
        ->set_text(var::NumberString(value));
    };

    container
      .add_event_callback(
        EventCode::notified,
        [](lv_event_t *e) {
          const Event event(e);

          Container object(event.parameter<lv_obj_t *>());
          set_property(x_name, object.get_x());
          set_property(y_name, object.get_y());
          set_property(width_name, object.get_width());

          set_property(
            height_name,
            object.get_property_value(Property::height).number());

          set_property(padding_left_name, object.get_left_padding());
          set_property(padding_right_name, object.get_right_padding());
          set_property(padding_top_name, object.get_top_padding());
          set_property(padding_bottom_name, object.get_bottom_padding());

          set_property(
            outline_padding,
            object.get_property_value(Property::outline_padding).number());
        })
      .add(E::Title("", "Properties"))
      .add(E::Spacer())
      .add(E::PropertySectionTitle("Position"))
      .add(E::Property(x_name, "x"))
      .add(E::Property(y_name, "y"))
      .add(E::Spacer())
      .add(E::PropertySectionTitle("Size"))
      .add(E::Property(width_name, "width"))
      .add(E::Property(height_name, "height"))
      .add(E::Spacer())
      .add(E::PropertySectionTitle("Content Area"))
      .add(E::Property(content_width_name, "width"))
      .add(E::Property(content_height_name, "height"))
      .add(E::Spacer())
      .add(E::PropertySectionTitle("Border"))
      .add(E::Property(border_left_name, "left"))
      .add(E::Property(border_right_name, "right"))
      .add(E::Property(border_top_name, "top"))
      .add(E::Property(border_bottom_name, "bottom"))
      .add(E::Spacer())
      .add(E::PropertySectionTitle("Outline"))
      .add(E::Property(outline_padding, "padding"))
      .add(E::Spacer())
      .add(E::PropertySectionTitle("Padding"))
      .add(E::Property(padding_left_name, "left"))
      .add(E::Property(padding_right_name, "right"))
      .add(E::Property(padding_top_name, "top"))
      .add(E::Property(padding_bottom_name, "bottom"));

    for (const auto property : property_list) {
      if ((property == Property::invalid) || (property == Property::any)) {
        continue;
      }
      const char *name = Style::to_cstring(property);
      container.add(E::Property(name, name));
    }
  }
};

#endif // PROPERTIES_HPP
