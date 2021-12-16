//
// Created by Tyler Gilbert on 12/15/21.
//

#include <fs.hpp>
#include <json.hpp>
#include <sys/System.hpp>
#include <var.hpp>

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
