//
// Created by Tyler Gilbert on 3/1/22.
//

#ifndef DESIGNLAB_EDITCOMPONENT_HPP
#define DESIGNLAB_EDITCOMPONENT_HPP

#include <design/extras/Form.hpp>
#include <design/macros.hpp>
#include <lvgl/ObjectAccess.hpp>

#include "AddComponent.hpp"

class EditComponent : public lvgl::ObjectAccess<EditComponent> {
public:
  struct Data : public lvgl::UserDataAccess<Data> {
    Data(const char *name) : lvgl::UserDataAccess<Data>(name) {}
    API_PMAZ(json_object, Data, json::JsonObject, {});
    API_PMAZ(target_object, Data, lvgl::Object, {});
  };

  design::Form get_form() {
    return find<design::Form>({Names::form_column, Names::form});
  }

  using Fields = AddComponent::Fields;

  struct PropertyFields {
    DESIGN_DECLARE_NAME(property);
    DESIGN_DECLARE_NAME(value);
    DESIGN_DECLARE_NAME(type);
  };

  explicit EditComponent(Data &data);
  LVGL_OBJECT_ACCESS_CONSTRUCT_FROM_OBJECT(EditComponent);

  static bool is_cancel_button(lv_event_t * e);

private:
  Data *data() { return user_data<Data>(); }

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

    DESIGN_DECLARE_NAME(main_row);
    DESIGN_DECLARE_NAME(control_column);
    DESIGN_DECLARE_NAME(form_column);
    DESIGN_DECLARE_NAME(property_row);
    DESIGN_DECLARE_NAME(cancel_button);

    DESIGN_DECLARE_NAME(lvgl_row);
    DESIGN_DECLARE_NAME(design_row);
    DESIGN_DECLARE_NAME(extra_design_row);

    DESIGN_DECLARE_NAME(form_hline_type_start);
    DESIGN_DECLARE_NAME(form_hline_type_stop);
  };

  static void add_property_clicked(lv_event_t * e);

  static EditComponent get_edit_component(lv_event_t * e);

  static void add_property_to_form(design::Form form, var::StringView property_name, s32 offset);

};

#endif // DESIGNLAB_EDITCOMPONENT_HPP
