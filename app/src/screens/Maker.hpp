//
// Created by Tyler Gilbert on 12/14/21.
//

#ifndef DESIGNLAB_MAKER_HPP
#define DESIGNLAB_MAKER_HPP

#include "ScreenObject.hpp"

class Maker : public ScreenObject {
protected:
  struct Names {
    static constexpr auto input_form_container = "InputFormContainer";
    static constexpr auto input_form_column = "InputFormColumn";
    static constexpr auto show_column = "showColumn";
  };

  static void hide_form(lv_event_t *) {
    screen()
      .find<Container>(Names::input_form_container)
      .clear_state(State::user1);
  }

  static void show_form(lv_event_t *) {
    screen().find<Container>(Names::input_form_container).add_state(State::user1);
    screen().find<Column>(Names::input_form_column).scroll_to_y(0, IsAnimate::no);
  }

  static void configure_form_animation(Generic generic) {
    generic.find<Container>(Names::input_form_container)
      .add_style("slide_over_from_right_hidden")
      .add_style("slide_over_from_right", State::user1);
  }
};

#endif // DESIGNLAB_MAKER_HPP
