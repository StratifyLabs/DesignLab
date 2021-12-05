//
// Created by Tyler Gilbert on 11/27/21.
//

#ifndef GUI_THEMEGENERATOR_HPP
#define GUI_THEMEGENERATOR_HPP

#include <fs/File.hpp>
#include <json/Json.hpp>
#include <lvgl/Types.hpp>
#include <printer/YamlPrinter.hpp>
#include <sys/Cli.hpp>
#include <var/StackString.hpp>
#include <var/Vector.hpp>

#include <cprinter/CPrinter.hpp>



class ThemeGenerator : public api::ExecutionContext {
public:
  ThemeGenerator(const sys::Cli &cli);
  printer::Printer &printer() { return m_printer; }

private:
  class CodePrinter : public cprinter::CPrinter {
  public:
    CodePrinter() = default;
    CodePrinter(const fs::FileObject &output_file)
      : m_output_file(&output_file) {}

    void interface_print_final(var::StringView value) override {
      API_ASSERT(m_output_file);
      m_output_file->write(value);
    }

  private:
    const fs::FileObject *m_output_file = nullptr;
  };

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
  fs::File m_output;
  var::PathString m_theme_path;


  json::JsonObject load_reference_json_file(var::StringView key);
  json::JsonObject load_json_file(var::StringView path);
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

  const char *get_lv_path_animation_path(Animation::Path value);

  void write_output(const var::GeneralString &value) {
    m_output.write(value.string_view());
  }
};

#endif // GUI_THEMEGENERATOR_HPP
