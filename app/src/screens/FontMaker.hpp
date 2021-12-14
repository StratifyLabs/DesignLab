//
// Created by Tyler Gilbert on 12/12/21.
//

#ifndef DESIGNLAB_FONTMAKER_HPP
#define DESIGNLAB_FONTMAKER_HPP

#include <design/extras/Form.hpp>

#include "ScreenObject.hpp"

class FontMaker : public ScreenObject {
public:
  static void configure(Generic generic);

  struct Names {
    static constexpr auto font_name = "FontName";
    static constexpr auto font_style = "FontStyle";
    static constexpr auto character_range = "CharacterRange";
    static constexpr auto bits_per_pixel = "BitsPerPixel";
    static constexpr auto sizes_section_heading = "sizesSectionHeading";
    static constexpr auto sizes_start = "sizesStart";
    static constexpr auto sizes_steps = "sizesSteps";
    static constexpr auto sizes_total = "sizesTotal";
    static constexpr auto select_file = "Select Font File";
  };

  class InputSchema : var::Vector<json::JsonObject> {
  public:
    InputSchema();

    static Form::Schema get_form_schema() {
      Form::Schema result;
      result.set_name("Fonts")
        .set_type(Form::Schema::schema_type)
        .set_input(InputSchema());
      return result;
    }
  };

private:
  static void configure_form_input(Form form);
};

#endif // DESIGNLAB_FONTMAKER_HPP
