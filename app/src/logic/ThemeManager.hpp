//
// Created by Tyler Gilbert on 11/27/21.
//

#ifndef DESIGNLAB_LOGIC_THEMEMANAGER_HPP
#define DESIGNLAB_LOGIC_THEMEMANAGER_HPP

#include <fs/File.hpp>
#include <fs/DataFile.hpp>
#include <json/Json.hpp>
#include <lvgl/Types.hpp>
#include <lvgl/Animation.hpp>
#include <printer/YamlPrinter.hpp>
#include <sys/Cli.hpp>
#include <var/StackString.hpp>
#include <var/Vector.hpp>

#include "Manager.hpp"

class ThemeManager : public Manager {
public:
  explicit ThemeManager(const sys::Cli &cli);

  struct Construct {
    API_PMAZ(input_path,Construct,var::PathString,{});
    API_PMAZ(project_path,Construct,var::PathString,{});
    API_PMAZ(output_source_path,Construct,var::PathString,{});
  };

  explicit ThemeManager(const Construct & options){
    construct(options);
  }

  fs::PathList get_source_list(var::StringView project_path, const Settings & settings) override;

  printer::Printer &printer() { return m_printer; }

private:

  class ThemeObject : public json::JsonValue {
  public:
    JSON_ACCESS_CONSTRUCT_OBJECT(ThemeObject);
    JSON_ACCESS_INTEGER(ThemeObject, version);
    JSON_ACCESS_STRING(ThemeObject, type);
    JSON_ACCESS_STRING(ThemeObject, colors);
    JSON_ACCESS_STRING(ThemeObject, sizes);
    JSON_ACCESS_STRING(ThemeObject, styles);
    JSON_ACCESS_STRING(ThemeObject, rules);
    JSON_ACCESS_STRING(ThemeObject, name);
    JSON_ACCESS_STRING(ThemeObject, directory);
    JSON_ACCESS_STRING(ThemeObject, header);
  };

  class RuleObject : public json::JsonValue {
  public:
    JSON_ACCESS_CONSTRUCT_OBJECT(RuleObject);
    JSON_ACCESS_STRING(RuleObject, condition);
    JSON_ACCESS_OBJECT(RuleObject,json::JsonObject,styles);
  };

  static constexpr auto lv_class_list = {
    "arc",
    "bar",
    "btn",
    "btnmatrix",
    "canvas",
    "checkbox",
    "calendar_header_arrow",
    "calendar_header_dropdown",
    "dropdown",
    "dropdownlist",
    "img",
    "label",
    "line",
    "roller",
    "slider",
    "switch",
    "table",
    "textarea",
    "calendar",
    "chart",
    "colorwheel",
    "keyboard",
    "imgbtn",
    "led",
    "list",
    "obj",
    "list_text",
    "list_btn",
    "meter",
    "msgbox",
    "span",
    "spinbox",
    "spinner",
    "tabview",
    "tileview",
    "tileview_tile",
    "win"};

  const sys::Cli *m_cli;
  CodePrinter m_code_printer;
  printer::YamlPrinter m_printer;
  ThemeObject m_theme_object;
  json::JsonObject m_variables_object;
  json::JsonObject m_styles_object;
  json::JsonObject m_rules_object;
  fs::DataFile m_output;
  var::PathString m_theme_path;


  void construct(const Construct & options);
  json::JsonObject load_reference_json_file(var::StringView key);
  void add_variables(var::StringView path);

  void generate_descriptors();
  void generate_styles();
  void generate_apply_callback();
  void generate_theme();

  const char *get_lv_style_from_name(const char *property_name);
  var::GeneralString
  get_property_value(lvgl::Property property, const char *value);
  var::GeneralString get_variable(var::StringView key);
  var::GeneralString get_json_value(const json::JsonValue value);
  var::GeneralString get_lv_state_part(var::StringView key_name);
  var::GeneralString get_condition(var::StringView condition_value);
  var::GeneralString get_effective_style(var::StringView value);

  const char *get_lv_path_animation_path(lvgl::Animation::Path value);

  void write_output(const var::GeneralString &value) {
    m_output.write(value.string_view());
  }
};

#endif // DESIGNLAB_LOGIC_THEMEMANAGER_HPP
