//
// Created by Tyler Gilbert on 12/14/21.
//

#ifndef DESIGNLAB_MAKER_HPP
#define DESIGNLAB_MAKER_HPP

#include "ScreenObject.hpp"

class Maker : public ScreenObject {
protected:
  static constexpr auto input_form_container_name = "InputFormContainer";
  static constexpr auto input_form_column_name = "InputFormColumn";

  static void hide_form(lv_event_t *) {
    screen()
      .find<Container>(input_form_container_name)
      .clear_state(State::user1);
  }

  static void show_form(lv_event_t *) {
    screen().find<Container>(input_form_container_name).add_state(State::user1);
    screen().find<Column>(input_form_column_name).scroll_to_y(0, IsAnimate::no);
  }

  static void configure_form_animation(Generic generic) {
    generic.find<Container>(input_form_container_name)
      .add_style("slide_over_from_right_hidden")
      .add_style("slide_over_from_right", State::user1);
  }
};

#endif // DESIGNLAB_MAKER_HPP
