//
// Created by Tyler Gilbert on 12/8/21.
//

#ifndef DESIGNLAB_THEMEMAKER_HPP
#define DESIGNLAB_THEMEMAKER_HPP

#include "Maker.hpp"

class ThemeMaker : public Maker {
public:
  static void configure(lvgl::Generic generic);

private:
  class InputSchema : var::Vector<json::JsonObject> {
  public:
    InputSchema();

    static Form::Schema get_form_schema() {
      Form::Schema result;
      result.set_name(Settings::fonts_key())
        .set_type(Form::Schema::schema_type)
        .set_input(InputSchema());
      return result;
    }
  };

  static void configure_form_input(Generic generic);
  static void handle_add(lv_event_t*e);
  static void show_themes(Column column);

};

#endif // DESIGNLAB_THEMEMAKER_HPP
