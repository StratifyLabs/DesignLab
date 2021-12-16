//
// Created by Tyler Gilbert on 12/14/21.
//

#include <json.hpp>
#include <var.hpp>

#include "Manager.hpp"

#include "screens/ScreenObject.hpp"
#include "Settings.hpp"

Settings &
Settings::import_form_schema(const design::Form form) {

  auto &printer = ScreenObject::printer();
  // pull in the form values to the project settings

  const auto schema_type_name = form.name();

  auto json_value = to_object().at(schema_type_name);
  if (!json_value.is_valid()) {
    // create the object if it doesn't already exist
    to_object().insert(schema_type_name, JsonArray());
    json_value = to_object().at(schema_type_name);
  }

  auto json_array = json_value.to_array();


  JsonObject object;
  for (const auto &child : form) {

    const StringView key_name = child.name();
    const StringView value = form.get_value(child);

    if (value.is_empty()) {
      // this is an error -- needs to be handled by the caller
      set_bad_key(key_name);
      API_RETURN_VALUE_ASSIGN_ERROR(
        *this,
        key_name | " does not have a value",
        EINVAL);
    } else if (value != Form::not_a_value) {
      object.insert(key_name, JsonString(value));
    }
  }

  json_array.append(object);
  return *this;
}

