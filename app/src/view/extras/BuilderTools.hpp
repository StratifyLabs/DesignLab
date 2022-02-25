//
// Created by Tyler Gilbert on 2/24/22.
//

#ifndef DESIGNLAB_BUILDERTOOLS_HPP
#define DESIGNLAB_BUILDERTOOLS_HPP

#include <design/macros.hpp>
#include <design/extras/Form.hpp>
#include <lvgl/ObjectAccess.hpp>

class BuilderTools : public lvgl::ObjectAccess<BuilderTools> {
public:
  LVGL_OBJECT_ACCESS_DECLARE_CONSTRUCTOR(BuilderTools);

  design::Form get_add_component_form(){
    return find<design::Form>(Names::form);
  }

  struct Fields {
    DESIGN_DECLARE_NAME(component_name);
    DESIGN_DECLARE_NAME(component_type);
    DESIGN_DECLARE_NAME(component_width);
    DESIGN_DECLARE_NAME(component_height);
  };

private:

  struct Names {
    DESIGN_DECLARE_NAME(top_container);
    DESIGN_DECLARE_NAME(parent_label);
    DESIGN_DECLARE_NAME(get_parent_button);
    DESIGN_DECLARE_NAME(get_previous_sibling_button);
    DESIGN_DECLARE_NAME(get_next_sibling_button);
    DESIGN_DECLARE_NAME(add_button);
    DESIGN_DECLARE_NAME(object_dropdown);
    DESIGN_DECLARE_NAME(column);
    DESIGN_DECLARE_NAME(form);
    DESIGN_DECLARE_NAME(fill_width_button);
    DESIGN_DECLARE_NAME(fill_height_button);
  };

  static void form_clicked(lv_event_t * e);
  static void fill_width_clicked(lv_event_t * e);
  static void fill_height_clicked(lv_event_t * e);

  static BuilderTools get_builder_tools(lv_event_t * e);

};

#endif // DESIGNLAB_BUILDERTOOLS_HPP
