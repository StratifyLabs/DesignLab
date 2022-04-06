//
// Created by Tyler Gilbert on 12/15/21.
//

#ifndef DESIGNLAB_LOGIC_SETTINGS_HPP
#define DESIGNLAB_LOGIC_SETTINGS_HPP

#include <fs/File.hpp>
#include <fs/FileSystem.hpp>
#include <json/Json.hpp>
#include <json/JsonDocument.hpp>
#include <sys/System.hpp>

#include <design/extras/Form.hpp>

class SessionSettings : public json::JsonValue {
public:
  static var::PathString get_application_directory();

  SessionSettings();
  ~SessionSettings();

  JSON_ACCESS_STRING(SessionSettings, project);
  JSON_ACCESS_STRING(SessionSettings, theme);
  JSON_ACCESS_INTEGER(SessionSettings, offset);
  JSON_ACCESS_STRING_WITH_KEY(SessionSettings, nodePath, node_path);
  JSON_ACCESS_STRING_WITH_KEY(SessionSettings, lvFontConvPath, lv_font_conv_path);
  JSON_ACCESS_INTEGER_WITH_KEY(SessionSettings, windowX, window_x);
  JSON_ACCESS_INTEGER_WITH_KEY(SessionSettings, windowY, window_y);

  bool is_project_path_valid() const {
    return fs::FileSystem().directory_exists(get_project());
  }

private:
  static var::PathString get_file_path() {
    return get_application_directory() / "designlab_session.json";
  }
};

class Settings : public json::JsonValue {
public:
  class Font : public json::JsonValue {
  public:
    JSON_ACCESS_CONSTRUCT_OBJECT(Font);
    JSON_ACCESS_STRING(Font, name);
    JSON_ACCESS_STRING(Font, path);
    JSON_ACCESS_STRING(Font, style);
    JSON_ACCESS_STRING(Font, range);
    JSON_ACCESS_STRING_WITH_KEY(Font, bitsPerPixel, bits_per_pixel);
    JSON_ACCESS_STRING_WITH_KEY(Font, userSizes, user_sizes);
    JSON_ACCESS_STRING_WITH_KEY(Font, themeSize, theme_sizes);
    // include the icons with this font
    JSON_ACCESS_BOOL(Font, icons);
  };

  class Asset : public json::JsonValue {
  public:
    JSON_ACCESS_CONSTRUCT_OBJECT(Asset);
    JSON_ACCESS_STRING(Asset, path);
  };

  class Icon : public json::JsonValue {
  public:
    JSON_ACCESS_CONSTRUCT_OBJECT(Icon);
    JSON_ACCESS_STRING(Icon, family);
    JSON_ACCESS_STRING(Icon, name);
    JSON_ACCESS_STRING(Icon, unicode);
  };

  class Icons : public json::JsonValue {
  public:
    JSON_ACCESS_CONSTRUCT_OBJECT(Icons);
    JSON_ACCESS_STRING(Icons, name);
    JSON_ACCESS_STRING(Icons, path);
    JSON_ACCESS_ARRAY(Icons, Icon, range);
  };

  class Component : public json::JsonValue {
  public:
    JSON_ACCESS_CONSTRUCT_OBJECT(Component);
    JSON_ACCESS_STRING(Component,name);
    JSON_ACCESS_OBJECT(Component,json::JsonObject,tree);
    JSON_ACCESS_BOOL_WITH_KEY(Component,generateWorker,generate_worker);
    JSON_ACCESS_BOOL_WITH_KEY(Component,generateData,generate_data);

    Component& trim_tree();

  private:
    void trim_leaf(json::JsonObject leaf);
  };

  class Theme : public json::JsonValue {
  public:
    JSON_ACCESS_CONSTRUCT_OBJECT(Theme);
    JSON_ACCESS_STRING(Theme, path);
  };

  Settings() : json::JsonValue(json::JsonObject()) {}
  Settings(const json::JsonObject &object) : json::JsonValue(object) {}
  Settings(const Settings&) = delete;
  Settings& operator=(const Settings&) = delete;

  Settings(Settings&&a) = default;

  Settings& operator=(Settings&&a) = default;

  using IsOverwrite = fs::File::IsOverwrite;

  static var::PathString get_file_path(var::StringView path) {
    return path / "designlab.json";
  }

  Settings(var::StringView path, IsOverwrite is_overwrite = IsOverwrite::no);

  Settings &append_form_entry(design::Form form);
  Settings &edit_from_form_entry(size_t offset, design::Form form);

  ~Settings() {
    save();
  }

  Settings& save();

  JSON_ACCESS_STRING(Settings, name);
  JSON_ACCESS_STRING(Settings, source);
  JSON_ACCESS_ARRAY(Settings, Font, fonts);
  JSON_ACCESS_ARRAY(Settings, Asset, assets);
  JSON_ACCESS_ARRAY(Settings, Theme, themes);
  JSON_ACCESS_ARRAY(Settings, Component, components);
  JSON_ACCESS_OBJECT(Settings, Icons, icons);
  JSON_ACCESS_BOOL_WITH_KEY(Settings, fontDirty, font_dirty);
  JSON_ACCESS_BOOL_WITH_KEY(Settings, assetsDirty, assets_dirty);
  JSON_ACCESS_BOOL_WITH_KEY(Settings, fontsCompressed, fonts_compressed);

  var::PathString get_output_directory() const {
    return get_source() / "designlab";
  }

  Settings& update_dirty_bits(const var::StringView form_name) {
    if (form_name == fonts_key()) {
      set_font_dirty(true);
    } else if (form_name == assets_key()) {
      set_assets_dirty(true);
    }
    return *this;
  }

private:
  API_AC(Settings, var::KeyString, bad_key);
  var::PathString m_path;
  IsOverwrite m_is_overwrite = IsOverwrite::no;

  void move(Settings & a){
    auto json = to_object();
    auto a_json = a.to_object();
    auto tmp = a_json;
    a.to_object() = json;
    json = tmp;

    std::swap(m_path, a.m_path);
    std::swap(m_is_overwrite, a.m_is_overwrite);
  }
};

#endif // DESIGNLAB_LOGIC_SETTINGS_HPP
