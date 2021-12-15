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



class Manager : public api::ExecutionContext {
public:
  class Settings : public json::JsonValue {
  public:

    class Font : public json::JsonValue {
    public:
      JSON_ACCESS_CONSTRUCT_OBJECT(Font);
      JSON_ACCESS_STRING(Font,name);
      JSON_ACCESS_STRING(Font,path);
      JSON_ACCESS_STRING(Font,style);
      JSON_ACCESS_STRING(Font,range);
      JSON_ACCESS_STRING_WITH_KEY(Font,bitsPerPixel,bits_per_pixel);
      JSON_ACCESS_STRING_WITH_KEY(Font,sizesStart,sizes_start);
      JSON_ACCESS_STRING_WITH_KEY(Font,sizesSteps,sizes_steps);
      JSON_ACCESS_STRING_WITH_KEY(Font,sizesTotal,sizes_total);
      //include the icons with this font
      JSON_ACCESS_BOOL(Font,icons);

    };

    class Asset : public json::JsonValue {
    public:
      JSON_ACCESS_CONSTRUCT_OBJECT(Asset);
      JSON_ACCESS_STRING(Asset,path);
    };

    class Icons : public json::JsonValue {
    public:
      JSON_ACCESS_CONSTRUCT_OBJECT(Icons);
      JSON_ACCESS_STRING(Icons,name);
      JSON_ACCESS_STRING(Icons,path);
      JSON_ACCESS_STRING_ARRAY(Icons,unicode);
    };

    class Theme : public json::JsonValue {
    public:
      JSON_ACCESS_CONSTRUCT_OBJECT(Theme);
      JSON_ACCESS_STRING(Theme,path);
    };


    Settings() : json::JsonValue(json::JsonObject()){}
    Settings(const json::JsonObject & object) : json::JsonValue(object){}

    using IsOverwrite = fs::File::IsOverwrite;

    Settings(var::StringView path, IsOverwrite is_overwrite) : m_path(path), m_is_overwrite(is_overwrite){
      if( !fs::FileSystem().exists(path) ){
        return;
      }

      to_object() = json::JsonDocument().load(fs::File(path));
    }

    Settings& import_form_schema(design::Form form);

    ~Settings(){
      if( bool(m_is_overwrite) ){
        json::JsonDocument().save(to_object(), fs::File(m_is_overwrite, m_path));
      }
    }

    JSON_ACCESS_STRING(Settings,name);
    JSON_ACCESS_ARRAY(Settings,Font,fonts);
    JSON_ACCESS_ARRAY(Settings,Asset,assets);
    JSON_ACCESS_ARRAY(Settings,Theme,themes);
    JSON_ACCESS_ARRAY(Settings,Icons,icons);

  private:
    API_AC(Settings,var::KeyString,bad_key);
    var::PathString m_path;
    IsOverwrite m_is_overwrite = IsOverwrite::no;
  };

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

  printer::Printer &printer() { return m_printer; }


private:
  printer::YamlPrinter m_printer;
};

#endif // DESIGNLAB_MANAGER_HPP
