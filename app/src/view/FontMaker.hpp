//
// Created by Tyler Gilbert on 12/12/21.
//

#ifndef DESIGNLAB_FONTMAKER_HPP
#define DESIGNLAB_FONTMAKER_HPP

#include <design/extras/Form.hpp>

#include "ViewObject.hpp"
#include "extras/Extras.hpp"

class FontMaker : public ViewObject, public ObjectAccess<FontMaker> {
public:
  static void configure(Generic generic){
    generic.add(FontMaker(ViewObject::Names::font_maker_object));
  }

private:
  LVGL_OBJECT_ACCESS_DECLARE_CONSTRUCTOR(FontMaker);
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

  static var::Vector<InfoCard::Data::Feature> get_feature_list(json::JsonObject);
  static var::StringView get_info_title(json::JsonObject object);


};

#endif // DESIGNLAB_FONTMAKER_HPP
