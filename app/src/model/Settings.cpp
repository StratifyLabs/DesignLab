//
// Created by Tyler Gilbert on 12/15/21.
//

#include <design/extras/Form.hpp>
#include <fs.hpp>
#include <json.hpp>
#include <sys/System.hpp>
#include <var.hpp>

#include <printer/Printer.hpp>

using namespace design;

#include "Settings.hpp"

var::PathString SessionSettings::get_application_directory() {
  API_ASSERT(is_success());
  const PathString result = sys::System().user_data_path() / "designlab";
  if (!FileSystem().directory_exists(result)) {
    FileSystem().create_directory(result, FileSystem::IsRecursive::yes);
  }
  return result;
}

SessionSettings::SessionSettings() {
  const auto path = get_file_path();
  if (!FileSystem().exists(path)) {
    printf("Session settings don't exist\n");
    to_object() = JsonObject();
    return;
  }
  to_object() = JsonDocument().load(File(path)).to_object();
}

SessionSettings::~SessionSettings() {
  api::ErrorScope error_scope;
  printf("Save session settings to %s\n", get_file_path().cstring());
  JsonDocument().save(
    to_object(),
    File(File::IsOverwrite::yes, get_file_path()));
}

Settings &Settings::append_form_entry(const design::Form form) {

  // pull in the form values to the project settings
  const auto form_name = form.name();
  auto json_value = to_object().at(form_name);
  if (!json_value.is_valid()) {
    // create the object if it doesn't already exist
    to_object().insert(form_name, JsonArray());
    json_value = to_object().at(form_name);
  }

  json_value.to_array().append(form.get_json_object());

  return *this;
}

Settings &
Settings::edit_from_form_entry(size_t offset, const design::Form form) {
  const auto form_name = form.name();
  auto json_value = to_object().at(form_name);
  printer::Printer().object("modify", json_value);

  API_ASSERT(json_value.is_valid());
  auto json_array = json_value.to_array();
  if (offset < json_array.count()) {
    auto form_object = form.get_json_object();
    printer::Printer().object("formObject", form_object);
    json_array.remove(offset).insert(offset, form.get_json_object());
  }
  printer::Printer().object("modified", json_array);

  return *this;
}

Settings &Settings::save() {
  if (bool(m_is_overwrite)) {
    printf("Save project settings to %s\n", m_path.cstring());
    json::JsonDocument().save(to_object(), fs::File(m_is_overwrite, m_path));
  }
  return *this;
}
