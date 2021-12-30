//
// Created by Tyler Gilbert on 12/12/21.
//

#ifndef DESIGNLAB_MANAGER_HPP
#define DESIGNLAB_MANAGER_HPP

#include <api/api.hpp>

#include <fs/FileSystem.hpp>
#include <fs/Path.hpp>
#include <json/Json.hpp>
#include <json/JsonDocument.hpp>
#include <printer/YamlPrinter.hpp>

#include <design/extras/Form.hpp>

#include "CodePrinter.hpp"

#include "model/Settings.hpp"


class Manager : public api::ExecutionContext {
public:

  bool is_create_file_ok(const var::StringView path) {
    const auto parent_path = fs::Path::parent_directory(path);
    if (!fs::FileSystem().directory_exists(parent_path)) {
      API_RETURN_VALUE_ASSIGN_ERROR(
        false,
        "cannot create " | path | " parent directory does not exist",
        EINVAL);
    }
    return true;
  }

  bool is_existing_file_ok(const var::StringView path) {
    if (!fs::FileSystem().exists(path)) {
      API_RETURN_VALUE_ASSIGN_ERROR(
        false,
        path | " does not exist",
        EINVAL);
    }
    return true;
  }

  json::JsonObject load_json_file(var::StringView path) {
    if(!is_existing_file_ok(path) ){
      return json::JsonObject();
    }

    json::JsonDocument document;
    const auto object = document.load(fs::File(path)).to_object();
    if (document.is_error()) {
      printer().object("jsonError", document.error());
      API_RETURN_VALUE_ASSIGN_ERROR(
        json::JsonObject(),
        "failed to parse `" | path | "`",
        EINVAL);
    }

    printer().object("json", object, printer::Printer::Level::message);
    return object;
  }

  virtual fs::PathList get_source_list(var::StringView project_path, const Settings & settings) {
    return {};
  }

  printer::Printer &printer() { return m_printer; }


private:
  printer::YamlPrinter m_printer;
};

#endif // DESIGNLAB_MANAGER_HPP
