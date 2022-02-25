//
// Created by Tyler Gilbert on 12/8/21.
//

#ifndef DESIGNLAB_THEMEMAKER_HPP
#define DESIGNLAB_THEMEMAKER_HPP

#include "ViewObject.hpp"
#include "extras/Extras.hpp"

class ThemeMaker : public ViewObject, public ObjectAccess<ThemeMaker> {
public:
  static void configure(lvgl::Generic generic){
    generic.add(ThemeMaker(ViewObject::Names::theme_maker_object));
  }

private:
  LVGL_OBJECT_ACCESS_DECLARE_CONSTRUCTOR(ThemeMaker);

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

#endif // DESIGNLAB_THEMEMAKER_HPP
