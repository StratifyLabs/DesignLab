//
// Created by Tyler Gilbert on 12/12/21.
//

#ifndef DESIGN_LAB_COMPONENT_MAKER_HPP
#define DESIGN_LAB_COMPONENT_MAKER_HPP

#include <design/extras/Form.hpp>

#include "ViewObject.hpp"
#include "extras/Extras.hpp"

class ComponentMaker : public ViewObject, public ObjectAccess<ComponentMaker> {
public:
  static void configure(Generic generic){
    generic.add(ComponentMaker(ViewObject::Names::component_maker_object));
  }
  LVGL_OBJECT_ACCESS_DECLARE_CONSTRUCTOR(ComponentMaker);

private:
  struct Names {
    static constexpr auto sizes_section_heading = "sizesSectionHeading";
    DESIGN_DECLARE_NAME(builder_drawer);
    DESIGN_DECLARE_NAME(new_component_prompt);
    DESIGN_DECLARE_NAME(new_component_form);
    DESIGN_DECLARE_NAME(new_component_line_field_name);
    DESIGN_DECLARE_NAME(new_component_modal);
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

  static void add_clicked(lv_event_t * e);
  static void edit_clicked(lv_event_t * e, u32 offset);
  static void builder_button_clicked(lv_event_t * e);

  static ComponentMaker get_self(lv_event_t * e);


};

#endif // DESIGN_LAB_COMPONENT_MAKER_HPP
