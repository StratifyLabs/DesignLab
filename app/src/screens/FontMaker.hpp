//
// Created by Tyler Gilbert on 12/12/21.
//

#ifndef DESIGNLAB_FONTMAKER_HPP
#define DESIGNLAB_FONTMAKER_HPP

#include <design/extras/Form.hpp>

#include "Maker.hpp"

class FontMaker : public Maker {
public:
  static void configure(Generic generic);

private:
  struct Names {
    static constexpr auto sizes_section_heading = "sizesSectionHeading";
  };

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
  static void show_fonts(Column column);

};

#endif // DESIGNLAB_FONTMAKER_HPP
