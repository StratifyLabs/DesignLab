//
// Created by Tyler Gilbert on 11/27/21.
//

#include <cprinter.hpp>
#include <fs.hpp>
#include <json.hpp>
#include <lvgl.hpp>
#include <printer.hpp>
#include <var.hpp>

#include <design/extras/Utility.hpp>

#include "ThemeManager.hpp"

using namespace design;

ThemeManager::ThemeManager(const sys::Cli &cli) : m_cli(&cli) {
  printer().set_verbose_level(cli.get_option("verbose"));
  m_construct = {.input_path = cli.get_option("theme"), .project_path = "./"};
  construct(m_construct);
}

void ThemeManager::construct(const Construct &options) {
  Printer::Array root_array(printer(), "DesignLabThemeGenerator");

  m_theme_path = options.project_path / options.input_path;
  if (m_theme_path.is_empty()) {
    API_RETURN_ASSIGN_ERROR(
      "`theme` must specify the path to the theme JSON file",
      EINVAL);
  }

  if (!FileSystem().exists(m_theme_path)) {
    API_RETURN_ASSIGN_ERROR(
      "theme `" | m_theme_path | "` does not exist",
      EINVAL);
  }

  m_theme_object = load_json_file(m_theme_path);
  if (m_theme_object.get_name().is_empty()) {
    API_RETURN_ASSIGN_ERROR(
      "`name` must be specified in the theme file",
      EINVAL);
  }

  // load sizes, colors, and style variables
  add_variables("sizes");
  add_variables("colors");
  add_variables("fonts");
  add_variables("styles");
  add_variables("rules");
  API_RETURN_IF_ERROR();

  {
    const auto object = load_reference_json_file("styles");
    m_styles_object = object.at("@styles").to_object();

    if (!m_styles_object.is_valid()) {
      API_RETURN_ASSIGN_ERROR(
        "`@styles` not found in " | m_theme_object.get_styles(),
        EINVAL);
    }
  }
  API_RETURN_IF_ERROR();

  {
    const auto object = load_reference_json_file("rules");
    m_rules_object = object.at("&rules").to_object();
    if (!m_rules_object.is_valid()) {
      API_RETURN_ASSIGN_ERROR(
        "`&rules` not found in " | m_theme_object.get_styles(),
        EINVAL);
    }
  }
  API_RETURN_IF_ERROR();

  const auto output_path
    = get_source_directory() / m_theme_object.get_name() & ".c";

  printer().key("outputFile", output_path);
  m_output = DataFile();

  API_RETURN_IF_ERROR();

  m_code_printer = CodePrinter(m_output);

  if (const auto header_value = m_theme_object.get_header();
      !header_value.is_empty()) {
    m_code_printer.header(header_value).newline();
  }

  generate_descriptors();
  generate_styles();
  generate_apply_callback();
  generate_theme();

  if (is_error()) {
    printer().object("error", error());
    return;
  }

  API_ASSERT(is_success());

  File(File::IsOverwrite::yes, output_path).write(m_output.seek(0));
}

var::PathString ThemeManager::get_source_directory() const {
  return m_construct.project_path / m_construct.output_source_path
         / "designlab/themes";
}

fs::PathList ThemeManager::get_source_list(
  var::StringView project_path,
  const Settings &settings) {
  API_RETURN_VALUE_IF_ERROR({});
  fs::PathList result;

  const auto theme_list = settings.get_themes();
  for (const auto &theme_file : theme_list) {
    ThemeObject theme = load_json_file(project_path / theme_file.get_path());
    result.push_back("themes" / theme.get_name() & ".c");
  }
  result.push_back("themes/themes.c").push_back("themes/themes.h");
  return result;
}

ThemeManager::ThemeNameContainer ThemeManager::get_name_list(
  var::StringView project_path,
  const Settings &settings) {
  API_RETURN_VALUE_IF_ERROR({});

  const auto theme_list = settings.get_themes();
  ThemeNameContainer result;
  result.reserve(theme_list.count());
  for (const auto &theme_file : theme_list) {
    ThemeObject theme = load_json_file(project_path / theme_file.get_path());
    result.push_back(theme.get_name());
  }
  return result;
}

var::NameString
ThemeManager::get_scaled_font_name(var::StringView font_variable_value) const {
  const auto font_components = font_variable_value.split("_");
  if (font_components.count() != 3) {
    return {};
  }

  const auto point_size = font_components.at(2).to_unsigned_long();
  const auto size
    = var::NumberString(u32(point_size * m_theme_object.get_y_scale()));

  return var::NameString(font_components.at(0))
    .append("_")
    .append(font_components.at(1))
    .append("_")
    .append(size);
}

fs::PathList ThemeManager::get_font_name_list() const {
  const auto key_container = m_variables_object.get_key_list();
  const auto font_count = [&]() {
    int count = 0;
    for (const auto &key : key_container) {
      if (key.find("$font") == 0) {
        ++count;
      }
    }
    return count;
  }();
  fs::PathList result;
  result.reserve(font_count);
  for (const auto &key : key_container) {
    if (key.find("$font") == 0) {
      const auto font_name = m_variables_object.at(key).to_string_view();
      const auto scaled_name = get_scaled_font_name(font_name).pop_front();
      if (!scaled_name.is_empty()) {
        result.push_back(scaled_name.string_view());
      }
    }
  }
  return result;
}

json::JsonObject ThemeManager::load_reference_json_file(var::StringView key) {
  const auto parent = Path::parent_directory(m_theme_path);
  const auto filename = m_theme_object.to_object().at(key).to_string_view();
  const auto path
    = parent.is_empty() ? PathString(filename) : parent / filename;

  return load_json_file(path);
}

void ThemeManager::add_variables(const StringView key) {
  API_RETURN_IF_ERROR();

  if (m_theme_object.to_object().at(key).is_string()) {
    const auto filename = m_theme_object.to_object().at(key).to_string_view();
    const auto parent = Path::parent_directory(m_theme_path);
    const auto path
      = parent.is_empty() ? PathString(filename) : parent / filename;

    if (!FileSystem().exists(path)) {
      API_RETURN_ASSIGN_ERROR(
        "variables file " | path | " does not exist",
        EINVAL);
    }

    JsonDocument document;
    const auto object = document.load(File(path)).to_object();
    if (document.is_error()) {
      printer().object("jsonError", document.error());
      API_RETURN_ASSIGN_ERROR("failed to parse json file " | path, EINVAL);
    }

    const auto list = object.at("$variables").to_object();
    if (!list.is_object()) {
      API_RETURN_ASSIGN_ERROR("did not find `$variables` in " | path, EINVAL);
    }

    const auto key_list = list.get_key_list();
    for (const auto &list_key : key_list) {
      m_variables_object.insert(list_key, list.at(list_key));
    }
  }
}

void ThemeManager::generate_apply_callback() {
  API_RETURN_IF_ERROR();

  const auto rule_key_list = m_rules_object.get_key_list();

  /*
   * The idea here is to make the theme loadable from a regular
   * file. This can't be done easily because
   * the themes rely on callback (checking condition,
   * applying color filters).
   *
   * The theme would have to load executable code which is a bit
   * problematic.
   *
   */
#if USE_STANDARDIZED_APPLY_CALLBACK
  // generate the code for the conditions
  m_code_printer.inline_comment("Condition callbacks");

  auto get_condition_callback_name = [](StringView rule_name) {
    return rule_name.pop_front() | "_condition_callback";
  };

  auto get_style_selector_pair_name
    = [](StringView state_selector, StringView style) {
        //"default | items": "button"
        const auto state_selector_list = state_selector.split("|");
        GeneralString result;
        for (const auto &item : state_selector_list) {
          result |= String(item).replace(
            String::Replace().set_old_string(" ").set_new_string(""));
          result |= "_";
        }
        if (result.length()) {
          result.pop_back();
        }
        return result | "_" | style;
      };

  for (const auto &key : rule_key_list) {
    const RuleObject rule(m_rules_object.at(key));
    {
      CPrinter::Function apply_callback_function(
        m_code_printer,
        "static int " | get_condition_callback_name(key)
          | "(lv_theme_t * theme, lv_obj_t * object)");

      {
        CPrinter::Scope condition_scope(
          m_code_printer,
          CPrinter::Scope::Type::if_,
          get_condition(rule.get_condition()));
        m_code_printer.statement("return 1");
      }

      m_code_printer.statement("return 0");
    }
    m_code_printer.newline();
  }
  m_code_printer.newline();

  Queue<GeneralString> style_selector_pair_list;
  for (const auto &key : rule_key_list) {
    const RuleObject rule(m_rules_object.at(key));
    const auto styles_object = rule.get_styles();
    const auto style_key_list = styles_object.get_key_list();
    for (const auto &style_key : style_key_list) {
      const auto style_value = styles_object.at(style_key);
      const auto style_value_string
        = style_value.is_string()
            ? GeneralString(style_value.to_string_view())
            : [&]() {
                GeneralString result;
                for (const auto &entry : style_value.to_array()) {
                  result |= entry.to_string_view() | "_";
                }
                if (result.length()) {
                  result.pop_back();
                }
                return result;
              }();
      const auto variable_name
        = get_style_selector_pair_name(style_key, style_value_string);
      const auto existing
        = style_selector_pair_list.find(variable_name, GeneralString());
      if (existing.is_empty()) {
        style_selector_pair_list.push(variable_name);
        CPrinter::StructInitialization(
          m_code_printer,
          "const lvgl_api_apply_style_with_selector_t " | variable_name)
          .add_member("style", "&" | variable_name)
          .add_member("selector", get_lv_state_part(style_key));
      }
    }
  }
#endif

  {
    CPrinter::Function apply_callback_function(
      m_code_printer,
      "void " | m_theme_object.get_name()
        | "_apply_callback(lv_theme_t * theme, lv_obj_t * object)");

    auto add_styles = [&](StringView class_name, JsonObject object) {
      CPrinter::Scope condition_scope(
        m_code_printer,
        CPrinter::Scope::Type::if_,
        get_condition(object.at("condition").to_string_view()));

      const auto styles_object = object.at("styles").to_object();
      const auto style_key_list = styles_object.get_key_list();
      for (const auto &key : style_key_list) {
        //"default"
        GeneralString line;
        const auto state_part = get_lv_state_part(key);

        const auto json_value = styles_object.at(key);
        if (json_value.is_array()) {
          for (const auto &value : json_value.to_array()) {
            const auto effective_style
              = get_effective_style(value.to_string_view());
            m_code_printer.statement(
              "lv_obj_add_style(object, (lv_style_t*)&" | effective_style
              | "_style, " | state_part | ")");
          }
        } else {
          const auto effective_style
            = get_effective_style(json_value.to_string_view());
          m_code_printer.statement(
            "lv_obj_add_style(object, (lv_style_t*)&" | effective_style
            | "_style, " | state_part | ")");
        }
      }
      m_code_printer.statement("return");
    };

    for (const auto &key : rule_key_list) {
      add_styles(key, m_rules_object.at(key).to_object());
    }
  }

  m_code_printer.newline();
}

void ThemeManager::generate_theme() {
  API_RETURN_IF_ERROR();
  const auto name = m_theme_object.get_name();

  const auto get_style_callback_name = name | "_get_style_callback";
  {
    CPrinter::Function get_style_function(
      m_code_printer,
      "const lvgl_api_style_descriptor_t * " | get_style_callback_name
        | "(int offset)");

    const auto list_name = name | "_style_descriptor_list";
    m_code_printer.statement(
      "const size_t size = sizeof(" | list_name
      | ") / sizeof(lvgl_api_style_descriptor_t)");
    {
      CPrinter::Scope offset_ok_scope(
        m_code_printer,
        CPrinter::Scope::Type::if_,
        "offset < size");
      m_code_printer.statement("return &(" | list_name | "[offset])");
    }
    m_code_printer.statement("return NULL");
  }
  m_code_printer.newline();

  CPrinter::StructInitialization(
    m_code_printer,
    "lv_theme_t themes_" | name)
    .add_member("apply_cb", name | "_apply_callback")
    .add_member("parent", "NULL")
    .add_member("user_data", "(void*)" | get_style_callback_name)
    .add_member("disp", "NULL")
    .add_member("color_primary", "0")
    .add_member("color_secondary", "0")
    .add_member("font_small", get_variable("$fontBodySmall"))
    .add_member("font_normal", get_variable("$fontBodyMedium"))
    .add_member("font_large", get_variable("$fontBodyLarge"))
    .add_member("flags", "0");
  m_code_printer.newline();

  {
    CPrinter::Function initialize_function(
      m_code_printer,
      "lv_theme_t * themes_" | name
        | "_initialize(lv_disp_t * disp, lv_theme_t * parent)");

    m_code_printer.statement("themes_" | name | ".disp = disp")
      .statement("themes_" | name | ".parent = parent")
      .statement("return &themes_" | name);
  }

  m_code_printer.newline();
}

void ThemeManager::generate_descriptors() {

  {
    const auto key_list = m_variables_object.get_key_list();
    for (const auto &key : key_list) {
      if (key.find("$font") == 0) {
        const auto font_variable_name
          = m_variables_object.at(key).to_string_view().pop_front();
        m_code_printer.statement(
          "extern const lv_font_t " | get_scaled_font_name(font_variable_name));
      }
    }
    m_code_printer.newline();
  }

  for (const auto &variable : m_variables_object) {
    if (variable.is_object()) {
      const auto json_object = variable.to_object();
      const auto type = json_object.at("type").to_string_view();
      const auto name = json_object.at("name").to_string_view();
      if (type == "transitionPropertyList") {
        const auto property_list = json_object.at("properties").to_array();

        {
          CPrinter::StructInitialization struct_init(
            m_code_printer,
            "static const lv_style_prop_t " | name | "[]");

          for (const auto &property : property_list) {
            struct_init.add_member_with_comment(
              var::NumberString(
                u32(
                  design::Utility::property_from_string(property.to_cstring())),
                " (lv_style_prop_t)0x%04X"),
              property.to_string_view());
          }
        }
        m_code_printer.newline();
      }
    }
  }

  for (const auto &variable : m_variables_object) {
    if (variable.is_object()) {
      const auto json_object = variable.to_object();
      const auto type = json_object.at("type").to_string_view();
      const auto name = json_object.at("name").to_string_view();
      if (type == "transitionPropertyList") {
        // this is already handled
      } else if (type == "transitionDescriptor") {
        const auto variable_name = var::GeneralString(name);
        const auto delay = get_json_value(json_object.at("delay"));
        const auto period = get_json_value(json_object.at("period"));
        const auto path_type = get_json_value(json_object.at("path"));
        const auto property_list
          = get_json_value(json_object.at("property_list"));

        CPrinter::StructInitialization(
          m_code_printer,
          "static const lv_style_transition_dsc_t " | variable_name)
          .add_member("props", property_list)
          .add_member("user_data", "NULL")
          .add_member(
            "path_xcb",
            StringView(get_lv_path_animation_path(
              Animation::path_from_string(path_type))))
          .add_member("time", period)
          .add_member("delay", delay);

      } else if (type == "colorFilterDescriptor") {
        const auto code = get_json_value(json_object.at("code"));
        const auto effective_name = StringView(name).pop_front();

        {
          CPrinter::Function color_function(m_code_printer, "static lv_color_t " | effective_name
                                                            | "_callback(const lv_color_filter_dsc_t * filter_descriptor, "
                                                            "lv_color_t color, lv_opa_t opacity)");
          m_code_printer.statement(code);
        }
        CPrinter::StructInitialization(
          m_code_printer,
          "static const lv_color_filter_dsc_t " | effective_name)
          .add_member("filter_cb", effective_name | "_callback");
        m_code_printer.newline();
      }
    }
  }
}

void ThemeManager::generate_styles() {
  API_RETURN_IF_ERROR();

  static auto create_property_entry = [](StringView key, StringView value) {
    return "{ .prop = " | key | ", .value = { " | value | " } }";
  };

  auto get_style_const_list_entry = [&](const char *key, const char *value) {
    const char *property_name = get_lv_style_from_name(key);
    const StringView property = property_name;
    if (property == "unknown") {

      // check for special cases
      if (StringView(key) == "padding") {

        const auto update_value
          = get_property_value(Property::top_padding, value);

        return create_property_entry("LV_STYLE_PAD_TOP", update_value) | ",\n  "
               | create_property_entry("LV_STYLE_PAD_BOTTOM", update_value)
               | ",\n  "
               | create_property_entry("LV_STYLE_PAD_LEFT", update_value)
               | ",\n  "
               | create_property_entry("LV_STYLE_PAD_RIGHT", update_value);
      }

      if (StringView(key) == "horizontal_padding") {
        const auto update_value
          = get_property_value(Property::left_padding, value);

        return create_property_entry("LV_STYLE_PAD_LEFT", update_value)
               | ",\n  "
               | create_property_entry("LV_STYLE_PAD_RIGHT", update_value);
      }

      if (StringView(key) == "vertical_padding") {
        const auto update_value
          = get_property_value(Property::top_padding, value);
        return create_property_entry("LV_STYLE_PAD_TOP", update_value) | ",\n  "
               | create_property_entry("LV_STYLE_PAD_BOTTOM", update_value);
      }

      API_RETURN_VALUE_ASSIGN_ERROR(
        var::GeneralString(),
        "`" | StringView(key) | "` does not map to an `LV_STYLE_*` value",
        EINVAL);
    }

    return create_property_entry(
      property_name,
      get_property_value(design::Utility::property_from_string(key), value));
  };

  const auto key_list = m_styles_object.get_key_list();

  /*
   * const lv_style_const_prop_t name_const_list[] = {
   *  { .prop = LV_STYLE_WIDTH, .value = { .color = 0xffffff } }
   * };
   *
   * const lv_style_t name_style = {
   *  .v_p = { .const_props = name_const_list },
   *  .prop1 = 0,
   *  .is_const = 1,
   *  .has_group = 0,
   *  .prop_cnt = VALUE
   * };
   *
   */

  for (const auto &entry : key_list) {
    Printer::Object po(printer(), "process " | entry);
    const auto entry_object = m_styles_object.at(entry).to_object();
    const auto entry_key_list = entry_object.get_key_list();
    printer().object("keys", entry_key_list);
    const auto style_name = get_effective_style(entry);

    {
      CPrinter::StructInitialization entry(
        m_code_printer,
        "static const lv_style_const_prop_t " | style_name.string_view()
          | "_const_list[]");

      for (const auto &key_entry : entry_key_list) {

        const auto entry_value = get_json_value(entry_object.at(key_entry));

        entry.add_member(
          get_style_const_list_entry(KeyString(key_entry), entry_value));

        API_RETURN_IF_ERROR();
      }

      entry.add_member(create_property_entry("LV_STYLE_PROP_INV", ".num = 0"));
    }
    m_code_printer.newline();

    {
      CPrinter::StructInitialization(
        m_code_printer,
        "static const lv_style_t " | style_name.string_view() | "_style")
        .add_member(
          "v_p",
          "{ .const_props = " | style_name.string_view() | "_const_list }")
        .add_member("has_group", "0xff")
        .add_member("is_const", "1");
    }
    m_code_printer.newline();
  }
  {
    CPrinter::StructInitialization style_descriptor_list(
      m_code_printer,
      "static const lvgl_api_style_descriptor_t " | m_theme_object.get_name()
        | "_style_descriptor_list[]");

    for (const auto &entry : key_list) {
      Printer::Object po(printer(), "process " | entry);
      const auto entry_object = m_styles_object.at(entry).to_object();
      const auto entry_key_list = entry_object.get_key_list();
      printer().object("keys", entry_key_list);
      const auto style_name = KeyString(entry);
      if (style_name.string_view().find("@") == 0) {
        const auto effective_style_name = get_effective_style(style_name);
        style_descriptor_list.add_member(
          "{ .name = \"" | effective_style_name | "\", .style = &"
          | effective_style_name | "_style }");
      }
    }
  }
  m_code_printer.newline();
}

var::GeneralString ThemeManager::get_variable(const StringView key) {
  if (key.find("$") == StringView::npos) {
    return key;
  }

  const auto key_list = m_variables_object.get_key_list();
  for (const auto &entry : key_list) {
    if (StringView(key) == entry) {
      const auto result = m_variables_object.at(entry);

      if (result.is_integer()) {
        const auto integer_value = [&]() {
          const auto value = result.to_integer();
          if (key.back() == 'X' || key.find("Width") != StringView::npos) {
            return int(value * m_theme_object.get_x_scale());
          }

          if (
            key.back() == 'Y' || key.find("Height") != StringView::npos
            || key.find("Radius") != StringView::npos) {
            return int(value * m_theme_object.get_y_scale());
          }

          return value;
        }();
        return NumberString(integer_value).string_view();
      }

      if (result.is_real()) {
        return NumberString(result.to_real()).string_view();
      }

      if (result.is_true()) {
        return "1";
      }

      if (result.is_false()) {
        return "0";
      }

      if (result.is_object()) {
        return result.to_object().at("name").to_cstring();
      }

      if (result.is_string()) {
        // check for recursive variables
        GeneralString recursive_result;
        if (result.to_string_view().find("$") != StringView::npos) {
          const auto token_list = result.to_string_view().split(" ");
          for (const auto &token : token_list) {
            if (token.find("$") == 0) {
              recursive_result |= get_variable(token) | " ";
            } else {
              recursive_result |= token | " ";
            }
          }
          return recursive_result.pop_back();
        }

        if (key.find("$font") == 0) {
          return get_scaled_font_name(result.to_string_view()).string_view();
        }

        return result.to_string_view();
      }

      return m_variables_object.at(entry).to_string_view();
    }
  }

  API_RETURN_VALUE_ASSIGN_ERROR(
    "",
    "could not find variable `" | StringView(key) | "`",
    EINVAL);
  return "";
}

var::GeneralString
ThemeManager::get_json_value(const json::JsonValue json_value) {
  if (json_value.is_string()) {

    const auto string_value = json_value.to_string_view();
    if (string_value.find("$") != StringView::npos) {
      const auto list = string_value.split(" ");
      var::GeneralString result;
      for (const auto &part : list) {
        result |= get_variable(part) | " ";
      }
      return result.pop_back();
    }

    return GeneralString(string_value);
  }

  if (json_value.is_integer()) {
    return GeneralString(NumberString(json_value.to_integer()).string_view());
  }

  if (json_value.is_real()) {
    return GeneralString(NumberString(json_value.to_real()).string_view());
  }

  if (json_value.is_true()) {
    return "1";
  }

  if (json_value.is_false()) {
    return "0";
  }

  return {};
}

var::GeneralString
ThemeManager::get_property_value(Property property, const char *value) {

  auto get_color = [&](const char *value) {
    if (const auto number_value = Color::palette_from_string(value);
        number_value != Palette::invalid) {
      const auto lv_color = Color::get_palette(number_value).get_color();
      return var::GeneralString().format(
        ".color = LV_COLOR_MAKE(0x%02x,0x%02x,0x%02x) ",
        LV_COLOR_GET_R(lv_color),
        LV_COLOR_GET_G(lv_color),
        LV_COLOR_GET_B(lv_color));
    }

    const StringView color_value = value;
    if (color_value.find("LV_COLOR_") == 0) {
      return var::GeneralString().format(".color = %s ", value);
    }

    const auto number_value
      = color_value.to_unsigned_long(StringView::Base::auto_);

    const lv_color_t lv_color = {.full = u32(number_value)};
    return var::GeneralString().format(
      ".color = LV_COLOR_MAKE(0x%02x,0x%02x,0x%02x) ",
      LV_COLOR_GET_R(lv_color),
      LV_COLOR_GET_G(lv_color),
      LV_COLOR_GET_B(lv_color));
  };

  auto get_number = [&](const char *value) {
    if (const auto number_value = Utility::text_decoration_from_string(value);
        number_value != TextDecoration::invalid) {
      return var::GeneralString().format(".num = 0x%X", u32(number_value));
    }

    if (const auto number_value = Utility::text_alignment_from_string(value);
        number_value != TextAlignment::invalid) {
      return var::GeneralString().format(".num = 0x%X", u32(number_value));
    }

    if (const auto number_value = Utility::border_side_from_string(value);
        number_value != BorderSide::invalid) {
      return var::GeneralString().format(".num = 0x%X", u32(number_value));
    }

    if (const auto number_value
        = Utility::gradient_direction_from_string(value);
        number_value != GradientDirection::invalid) {
      return var::GeneralString().format(".num = 0x%X", u32(number_value));
    }

    if (const auto number_value = Utility::base_direction_from_string(value);
        number_value != BaseDirection::invalid) {
      return var::GeneralString().format(".num = 0x%X", u32(number_value));
    }

    if (const auto number_value = Utility::direction_from_string(value);
        number_value != Direction::invalid) {
      return var::GeneralString().format(".num = 0x%X", u32(number_value));
    }

    if (const auto number_value = Utility::alignment_from_string(value);
        number_value != Alignment::invalid) {
      return var::GeneralString().format(".num = 0x%X", u32(number_value));
    }

    if (const auto number_value = Utility::flex_flow_from_string(value);
        number_value != FlexFlow::invalid) {
      return var::GeneralString().format(".num = 0x%X", u32(number_value));
    }

    if (const auto number_value = Utility::flex_align_from_string(value);
        number_value != FlexAlign::invalid) {
      return var::GeneralString().format(".num = 0x%X", u32(number_value));
    }

    if (const auto number_value = Utility::scroll_bar_mode_from_string(value);
        number_value != ScrollBarMode::invalid) {
      return var::GeneralString().format(".num = 0x%X", u32(number_value));
    }

    if (const auto number_value = Utility::scroll_snap_mode_from_string(value);
        number_value != ScrollSnap::invalid) {
      return var::GeneralString().format(".num = 0x%X", u32(number_value));
    }

    if (const auto number_value = Utility::is_animate_mode_from_string(value);
        number_value != IsAnimate::invalid) {
      return var::GeneralString().format(".num = 0x%X", u32(number_value));
    }

    if (const auto number_value = Utility::blend_mode_from_string(value);
        number_value != BlendMode::invalid) {
      return var::GeneralString().format(".num = 0x%X", u32(number_value));
    }

    if (const auto number_value = Utility::opacity_from_string(value);
        number_value != Opacity::invalid) {
      return var::GeneralString().format(".num = 0x%X", u32(number_value));
    }

    if (value == StringView("true")) {
      return var::GeneralString(".num = 1");
    }

    if (value == StringView("false")) {
      return var::GeneralString(".num = 0");
    }

    return var::GeneralString().format(".num = %s", value);
  };

  auto get_pointer = [&](const char *value) {
    return var::GeneralString().format(".ptr = (void*)%s", value);
  };

  auto get_pointer_address = [&](const char *value) {
    return var::GeneralString().format(".ptr = (void*)&%s", value);
  };

  switch (property) {

  case Property::background_color:
  case Property::background_color_filtered:
  case Property::background_gradient_color:
  case Property::background_gradient_color_filtered:
  case Property::border_color:
  case Property::border_color_filtered:
  case Property::outline_color:
  case Property::outline_color_filtered:
  case Property::shadow_color:
  case Property::shadow_color_filtered:
  case Property::line_color:
  case Property::line_color_filtered:
  case Property::arc_color:
  case Property::arc_color_filtered:
  case Property::text_color:
  case Property::text_color_filtered:
    return get_color(value);

  case Property::text_font:
  case Property::arc_image_source:
  case Property::background_image_source:
  case Property::color_filter_descriptor:
    return get_pointer(value);
  case Property::transition:
    return get_pointer_address(value);

  case Property::width:
  case Property::minimum_width:
  case Property::maximum_width:
  case Property::height:
  case Property::minimum_height:
  case Property::maximum_height:
  case Property::x:
  case Property::y:
  case Property::alignment:
  case Property::transform_width:
  case Property::transform_height:
  case Property::translate_x:
  case Property::translate_y:
  case Property::transform_zoom:
  case Property::transform_angle:
  case Property::top_padding:
  case Property::bottom_padding:
  case Property::left_padding:
  case Property::right_padding:
  case Property::row_padding:
  case Property::column_padding:
  case Property::background_opacity:
  case Property::background_gradient_direction:
  case Property::background_gradient_stop:
  case Property::background_image_opacity:
  case Property::background_image_recolor_opacity:
  case Property::background_image_tiled:
  case Property::background_main_stop:
  case Property::background_image_recolor:
  case Property::background_image_recolor_filtered:
  case Property::border_opacity:
  case Property::border_width:
  case Property::border_side:
  case Property::border_post:
  case Property::outline_width:
  case Property::outline_opacity:
  case Property::outline_padding:
  case Property::shadow_width:
  case Property::shadow_x_offset:
  case Property::shadow_y_offset:
  case Property::shadow_spread:
  case Property::shadow_opacity:
  case Property::image_opacity:
  case Property::image_recolor:
  case Property::image_recolor_filtered:
  case Property::image_recolor_opacity:
  case Property::line_width:
  case Property::line_dash_width:
  case Property::line_dash_gap:
  case Property::line_rounded:
  case Property::line_opacity:
  case Property::arc_width:
  case Property::arc_rounded:
  case Property::arc_opacity:
  case Property::text_opacity:
  case Property::text_letter_spacing:
  case Property::text_line_spacing:
  case Property::text_decoration:
  case Property::text_alignment:
  case Property::radius:
  case Property::clip_corner:
  case Property::opacity:
  case Property::color_filter_opacity:
  case Property::animation_time:
  case Property::animation_speed:
  case Property::blend_mode:
  case Property::layout:
  case Property::base_direction:
    return get_number(value);

  case Property::invalid:
  case Property::any:
    break;
  }
  return {};
}

const char *ThemeManager::get_lv_path_animation_path(Animation::Path value) {
  switch (value) {
  case Animation::Path::linear:
    return "lv_anim_path_linear";
  case Animation::Path::ease_in:
    return "lv_anim_path_ease_in";
  case Animation::Path::ease_out:
    return "lv_anim_path_ease_out";
  case Animation::Path::ease_in_out:
    return "lv_anim_path_ease_in_out";
  case Animation::Path::bounce:
    return "lv_anim_path_bounce";
  case Animation::Path::overshoot:
    return "lv_anim_path_overshoot";
  case Animation::Path::step:
    return "lv_anim_path_step";
  case Animation::Path::invalid:
    return "lv_anim_path_linear";
  }
  return "NULL";
}

const char *ThemeManager::get_lv_style_from_name(const char *property_name) {
  const var::StringView property = property_name;
  if (property == "invalid") {
    return "LV_STYLE_PROP_INV";
  }
  if (property == "width") {
    return "LV_STYLE_WIDTH";
  }
  if (property == "minimum_width") {
    return "LV_STYLE_MIN_WIDTH";
  }
  if (property == "maximum_width") {
    return "LV_STYLE_MAX_WIDTH";
  }
  if (property == "height") {
    return "LV_STYLE_HEIGHT";
  }
  if (property == "minimum_height") {
    return "LV_STYLE_MIN_HEIGHT";
  }
  if (property == "maximum_height") {
    return "LV_STYLE_MAX_HEIGHT";
  }
  if (property == "x") {
    return "LV_STYLE_X";
  }
  if (property == "y") {
    return "LV_STYLE_Y";
  }
  if (property == "alignment") {
    return "LV_STYLE_ALIGN";
  }
  if (property == "transform_width") {
    return "LV_STYLE_TRANSFORM_WIDTH";
  }
  if (property == "transform_height") {
    return "LV_STYLE_TRANSFORM_HEIGHT";
  }
  if (property == "translate_x") {
    return "LV_STYLE_TRANSLATE_X";
  }
  if (property == "translate_y") {
    return "LV_STYLE_TRANSLATE_Y";
  }
  if (property == "transform_zoom") {
    return "LV_STYLE_TRANSFORM_ZOOM";
  }
  if (property == "transform_angle") {
    return "LV_STYLE_TRANSFORM_ANGLE";
  }
  if (property == "top_padding") {
    return "LV_STYLE_PAD_TOP";
  }
  if (property == "bottom_padding") {
    return "LV_STYLE_PAD_BOTTOM";
  }
  if (property == "left_padding") {
    return "LV_STYLE_PAD_LEFT";
  }
  if (property == "right_padding") {
    return "LV_STYLE_PAD_RIGHT";
  }
  if (property == "row_padding") {
    return "LV_STYLE_PAD_ROW";
  }
  if (property == "column_padding") {
    return "LV_STYLE_PAD_COLUMN";
  }
  if (property == "background_color") {
    return "LV_STYLE_BG_COLOR";
  }
  if (property == "background_color_filtered") {
    return "LV_STYLE_BG_COLOR_FILTERED";
  }
  if (property == "background_opacity") {
    return "LV_STYLE_BG_OPA";
  }
  if (property == "background_gradient_color") {
    return "LV_STYLE_BG_GRAD_COLOR";
  }
  if (property == "background_gradient_color_filtered") {
    return "LV_STYLE_BG_GRAD_COLOR_FILTERED";
  }
  if (property == "background_gradient_direction") {
    return "LV_STYLE_BG_GRAD_DIR";
  }
  if (property == "background_main_stop") {
    return "LV_STYLE_BG_MAIN_STOP";
  }
  if (property == "background_gradient_stop") {
    return "LV_STYLE_BG_GRAD_STOP";
  }
  if (property == "background_image_source") {
    return "LV_STYLE_BG_IMG_SRC";
  }
  if (property == "background_image_opacity") {
    return "LV_STYLE_BG_IMG_OPA";
  }
  if (property == "background_image_recolor") {
    return "LV_STYLE_BG_IMG_RECOLOR";
  }
  if (property == "background_image_recolor_filtered") {
    return "LV_STYLE_BG_IMG_RECOLOR_FILTERED";
  }
  if (property == "background_image_recolor_opacity") {
    return "LV_STYLE_BG_IMG_RECOLOR_OPA";
  }
  if (property == "background_image_tiled") {
    return "LV_STYLE_BG_IMG_TILED";
  }
  if (property == "border_color") {
    return "LV_STYLE_BORDER_COLOR";
  }
  if (property == "border_color_filtered") {
    return "LV_STYLE_BORDER_COLOR_FILTERED";
  }
  if (property == "border_opacity") {
    return "LV_STYLE_BORDER_OPA";
  }
  if (property == "border_width") {
    return "LV_STYLE_BORDER_WIDTH";
  }
  if (property == "border_side") {
    return "LV_STYLE_BORDER_SIDE";
  }
  if (property == "border_post") {
    return "LV_STYLE_BORDER_POST";
  }
  if (property == "outline_width") {
    return "LV_STYLE_OUTLINE_WIDTH";
  }
  if (property == "outline_color") {
    return "LV_STYLE_OUTLINE_COLOR";
  }
  if (property == "outline_color_filtered") {
    return "LV_STYLE_OUTLINE_COLOR_FILTERED";
  }
  if (property == "outline_opacity") {
    return "LV_STYLE_OUTLINE_OPA";
  }
  if (property == "outline_padding") {
    return "LV_STYLE_OUTLINE_PAD";
  }
  if (property == "shadow_width") {
    return "LV_STYLE_SHADOW_WIDTH";
  }
  if (property == "shadow_x_offset") {
    return "LV_STYLE_SHADOW_OFS_X";
  }
  if (property == "shadow_y_offset") {
    return "LV_STYLE_SHADOW_OFS_Y";
  }
  if (property == "shadow_spread") {
    return "LV_STYLE_SHADOW_SPREAD";
  }
  if (property == "shadow_color") {
    return "LV_STYLE_SHADOW_COLOR";
  }
  if (property == "shadow_color_filtered") {
    return "LV_STYLE_SHADOW_COLOR_FILTERED";
  }
  if (property == "shadow_opacity") {
    return "LV_STYLE_SHADOW_OPA";
  }

  if (property == "image_opacity") {
    return "LV_STYLE_IMG_OPA";
  }
  if (property == "image_recolor") {
    return "LV_STYLE_IMG_RECOLOR";
  }
  if (property == "image_recolor_filtered") {
    return "LV_STYLE_IMG_RECOLOR_FILTERED";
  }
  if (property == "image_recolor_opacity") {
    return "LV_STYLE_IMG_RECOLOR_OPA";
  }

  if (property == "line_width") {
    return "LV_STYLE_LINE_WIDTH";
  }
  if (property == "line_dash_width") {
    return "LV_STYLE_LINE_DASH_WIDTH";
  }
  if (property == "line_dash_gap") {
    return "LV_STYLE_LINE_DASH_GAP";
  }
  if (property == "line_rounded") {
    return "LV_STYLE_LINE_ROUNDED";
  }
  if (property == "line_color") {
    return "LV_STYLE_LINE_COLOR";
  }
  if (property == "line_color_filtered") {
    return "LV_STYLE_LINE_COLOR_FILTERED";
  }
  if (property == "line_opacity") {
    return "LV_STYLE_LINE_OPA";
  }

  if (property == "arc_width") {
    return "LV_STYLE_ARC_WIDTH";
  }
  if (property == "arc_rounded") {
    return "LV_STYLE_ARC_ROUNDED";
  }
  if (property == "arc_color") {
    return "LV_STYLE_ARC_COLOR";
  }
  if (property == "arc_color_filtered") {
    return "LV_STYLE_ARC_COLOR_FILTERED";
  }
  if (property == "arc_opacity") {
    return "LV_STYLE_ARC_OPA";
  }
  if (property == "arc_image_source") {
    return "LV_STYLE_ARC_IMG_SRC";
  }

  if (property == "text_color") {
    return "LV_STYLE_TEXT_COLOR";
  }
  if (property == "text_color_filtered") {
    return "LV_STYLE_TEXT_COLOR_FILTERED";
  }
  if (property == "text_opacity") {
    return "LV_STYLE_TEXT_OPA";
  }
  if (property == "text_font") {
    return "LV_STYLE_TEXT_FONT";
  }
  if (property == "text_letter_spacing") {
    return "LV_STYLE_TEXT_LETTER_SPACE";
  }
  if (property == "text_line_spacing") {
    return "LV_STYLE_TEXT_LINE_SPACE";
  }
  if (property == "text_decoration") {
    return "LV_STYLE_TEXT_DECOR";
  }
  if (property == "text_alignment") {
    return "LV_STYLE_TEXT_ALIGN";
  }

  if (property == "radius") {
    return "LV_STYLE_RADIUS";
  }
  if (property == "clip_corner") {
    return "LV_STYLE_CLIP_CORNER";
  }
  if (property == "opacity") {
    return "LV_STYLE_OPA";
  }
  if (property == "color_filter_descriptor") {
    return "LV_STYLE_COLOR_FILTER_DSC";
  }
  if (property == "color_filter_opacity") {
    return "LV_STYLE_COLOR_FILTER_OPA";
  }
  if (property == "animation_time") {
    return "LV_STYLE_ANIM_TIME";
  }
  if (property == "animation_speed") {
    return "LV_STYLE_ANIM_SPEED";
  }
  if (property == "transition") {
    return "LV_STYLE_TRANSITION";
  }
  if (property == "blend_mode") {
    return "LV_STYLE_BLEND_MODE";
  }
  if (property == "layout") {
    return "LV_STYLE_LAYOUT";
  }
  if (property == "base_direction") {
    return "LV_STYLE_BASE_DIR";
  }

  if (property == "any") {
    return "LV_STYLE_PROP_ANY";
  }

  return "unknown";
}

var::GeneralString ThemeManager::get_lv_state_part(var::StringView key_name) {
  GeneralString result;
  const auto modifier_list = key_name.split("|");
  for (const auto &item : modifier_list) {
    if (item.find("default") != StringView::npos) {
      result |= "LV_STATE_DEFAULT|";
    } else if (item.find("checked") != StringView::npos) {
      result |= "LV_STATE_CHECKED|";
    } else if (item.find("focused") != StringView::npos) {
      result |= "LV_STATE_FOCUSED|";
    } else if (item.find("focus_key") != StringView::npos) {
      result |= "LV_STATE_FOCUS_KEY|";
    } else if (item.find("edited") != StringView::npos) {
      result |= "LV_STATE_EDITED|";
    } else if (item.find("hovered") != StringView::npos) {
      result |= "LV_STATE_HOVERED|";
    } else if (item.find("pressed") != StringView::npos) {
      result |= "LV_STATE_PRESSED|";
    } else if (item.find("scrolled") != StringView::npos) {
      result |= "LV_STATE_SCROLLED|";
    } else if (item.find("disabled") != StringView::npos) {
      result |= "LV_STATE_DISABLED|";
    } else if (item.find("user1") != StringView::npos) {
      result |= "LV_STATE_USER_1|";
    } else if (item.find("user2") != StringView::npos) {
      result |= "LV_STATE_USER_2|";
    } else if (item.find("user3") != StringView::npos) {
      result |= "LV_STATE_USER_3|";
    } else if (item.find("user4") != StringView::npos) {
      result |= "LV_STATE_USER_4|";
    } else if (item.find("scrollbar") != StringView::npos) {
      result |= "LV_PART_SCROLLBAR|";
    } else if (item.find("indicator") != StringView::npos) {
      result |= "LV_PART_INDICATOR|";
    } else if (item.find("selected") != StringView::npos) {
      result |= "LV_PART_SELECTED|";
    } else if (item.find("knob") != StringView::npos) {
      result |= "LV_PART_KNOB|";
    } else if (item.find("items") != StringView::npos) {
      result |= "LV_PART_ITEMS|";
    } else if (item.find("ticks") != StringView::npos) {
      result |= "LV_PART_TICKS|";
    } else if (item.find("cursor") != StringView::npos) {
      result |= "LV_PART_CURSOR|";
    }
  }

  result.pop_back();
  return result;
}

var::GeneralString ThemeManager::get_effective_style(var::StringView value) {
  if (value.find("@") == 0) {
    return {value.pop_front()};
  }
  return {value};
}

var::GeneralString
ThemeManager::get_condition(var::StringView condition_value) {
  // replace .btn || .Button -> (lv_obj_check_type(object, &lv_btn_class))
  // replace #abcd (u32*)user_data == ('a' << 24 | 'b' << 16 | 'c' << 8 | 'd')

  const auto token_list = condition_value.split(" ");

  auto check_class_match = [](StringView token) -> GeneralString {
    for (const auto *class_name : lv_class_list) {
      {
        const auto match = StringView(".") | class_name;
        if (token == match.string_view()) {
          return "(lv_obj_check_type(object, &lv_" | StringView(class_name)
                 | "_class))";
        }
      }
      {
        const auto match = StringView(".parent(") | class_name | ")";
        if (token == match.string_view()) {
          return "(lv_obj_check_type(lv_obj_get_parent(object), &lv_"
                 | StringView(class_name) | "_class))";
        }
      }
      {
        const auto match = StringView(".grandparent(") | class_name | ")";
        if (token == match.string_view()) {
          return "(lv_obj_check_type(lv_obj_get_parent(lv_obj_get_parent("
                 "object)), &lv_"
                 | StringView(class_name) | "_class))";
        }
      }
    }

    // return the token unchanged if there is no match
    return GeneralString(token);
  };

  auto check_id_match = [](StringView token) -> GeneralString {
    if (token.at(0) == '#') {
      if (token.length() != 5) {
        API_RETURN_VALUE_ASSIGN_ERROR(
          "",
          token | " must be of the format `#<abcd>`, e.g. `#seco`",
          EINVAL);
      }
      return (
        "object->user_data && ((uint32_t*)(object->user_data))[0] == "
        | NumberString(
          token.at(1) | token.at(2) << 8 | token.at(3) << 16
            | token.at(4) << 24,
          "0x%08X"));
    }
    return GeneralString(token);
  };

  String condition = String(condition_value);
  for (const auto &token : token_list) {
    const auto class_match = check_class_match(token);
    const auto screen_match
      = class_match.string_view() == ".screen"
          ? GeneralString("(lv_obj_get_parent(object) == NULL)")
          : class_match;
    const auto id_match = check_id_match(screen_match);
    if (token != id_match.string_view()) {
      condition.replace(
        String::Replace().set_old_string(token).set_new_string(id_match));
    }
  }

  return GeneralString(condition.string_view());
}

void ThemeManager::generate_theme_source(
  const var::StringView output_directory,
  const ThemeNameContainer &theme_name_container) {

  File themes_h(File::IsOverwrite::yes, output_directory / "themes.h");
  File themes_c(File::IsOverwrite::yes, output_directory / "themes.c");

  CodePrinter h_printer(themes_h);
  CodePrinter c_printer(themes_c);

  const GeneralString lvgl_theme_list
    = "static const lvgl_api_theme_descriptor_t themes_list["
      | NumberString(theme_name_container.count()) | "]";

  {
    CodePrinter::HeaderGuard header_guard(
      h_printer,
      "DESIGNLAB_THEMES_THEMES_H_");
    h_printer.newline();
    h_printer.line("#include <lvgl_api.h>")
      .newline()
      .line("#if defined __cplusplus")
      .line("extern \"C\" {")
      .line("#endif")
      .newline()
      .line("const lvgl_api_theme_descriptor_t *themes_get_theme(int offset);")
      .line("#if defined __link")
      .line("void themes_initialize();")
      .line("#endif")
      .newline()
      .line("#if defined __cplusplus")
      .line("}")
      .line("#endif")
      .newline();
  }

  c_printer.newline().newline();

  c_printer.line("#include \"themes.h\"").newline().newline();

  static auto get_abbreviated_string = [](var::StringView style) {
    return lvgl::Font::to_abbreviated_cstring(
      lvgl::Font::style_from_string(style));
  };

  c_printer.newline().newline();

  auto get_text_name = [](var::StringView name) {
    return PathString(name).replace(
      PathString::Replace().set_old_character('_').set_new_character('-'));
  };

  for (const auto &name : theme_name_container) {
    c_printer.statement("extern const lv_theme_t themes_" | name);
  }

  c_printer.newline().newline();
  {
    CodePrinter::StructInitialization struct_init(c_printer, lvgl_theme_list);
    for (const auto &name : theme_name_container) {
      struct_init.add_member(
        "{ .name = \"" | get_text_name(name) | "\", .theme = &themes_" | name | "}");
    }
  }

  c_printer.newline().newline();
  {
    CodePrinter::Function function(
      c_printer,
      "const lvgl_api_theme_descriptor_t *themes_get_theme(int offset)");

    c_printer.statement(
      "const int count = sizeof(themes_list) / "
      "sizeof(lvgl_api_theme_descriptor_t)");

    {
      CodePrinter::IfScope check_offset(
        c_printer,
        "offset >= 0 && offset < count");
      c_printer.statement("return themes_list + offset");
    }
    c_printer.statement("return NULL");
  }
  for (const auto &name : theme_name_container) {
    c_printer.statement(
      "extern lv_theme_t * themes_" | name
      | "_initialize(lv_disp_t * disp, lv_theme_t * parent)");
  }

  c_printer.newline();

  {
    CodePrinter::Function function(c_printer, "void themes_initialize()");
    c_printer.line("#if defined __link");
    c_printer.statement("lvgl_api_set_theme_callback(themes_get_theme)");
    c_printer.line("#endif").newline();

    for (const auto &name : theme_name_container) {
      c_printer.statement(
        "themes_" | name | "_initialize(lv_disp_get_default(), NULL)");
    }
  }

  c_printer.newline();
}
