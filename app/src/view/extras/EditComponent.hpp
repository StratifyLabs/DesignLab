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

  design::Form get_form();
  using Fields = AddComponent::Fields;
  struct PropertyFields {
    DESIGN_DECLARE_NAME(property);
    DESIGN_DECLARE_NAME(value);
    DESIGN_DECLARE_NAME(type);
  };
  explicit EditComponent(Data &data);
  LVGL_OBJECT_ACCESS_CONSTRUCT_FROM_OBJECT(EditComponent);

  static bool is_cancel_button(lv_event_t * e);

  Data *data() { return user_data<Data>(); }


};

#endif // DESIGNLAB_EDITCOMPONENT_HPP
