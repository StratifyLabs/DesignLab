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
    JSON_ACCESS_STRING_WITH_KEY(Font, sizesStart, sizes_start);
    JSON_ACCESS_STRING_WITH_KEY(Font, sizesSteps, sizes_steps);
    JSON_ACCESS_STRING_WITH_KEY(Font, sizesTotal, sizes_total);
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

  class Theme : public json::JsonValue {
  public:
    JSON_ACCESS_CONSTRUCT_OBJECT(Theme);
    JSON_ACCESS_STRING(Theme, path);
  };

  Settings() : json::JsonValue(json::JsonObject()) {}
  Settings(const json::JsonObject &object) : json::JsonValue(object) {}

  using IsOverwrite = fs::File::IsOverwrite;

  static var::PathString get_file_path(var::StringView path) {
    return path / "designlab.json";
  }

  Settings(var::StringView path, IsOverwrite is_overwrite = IsOverwrite::no)
    : m_path(path), m_is_overwrite(is_overwrite) {
    printf("load project %s\n", var::PathString(path).cstring());
    if (!fs::FileSystem().exists(path)) {
      printf("doesn't exist\n");
      return;
    }

    api::ErrorScope error_scope;
    to_object() = json::JsonDocument().load(fs::File(path));
    if (is_error()) {
      printf("Couldn't load\n");
      reset_error();
      to_object() = json::JsonObject();
    }
  }

  Settings &append_form_entry(design::Form form);
  Settings &edit_from_form_entry(size_t offset, design::Form form);

  ~Settings() {
    if (bool(m_is_overwrite)) {
      json::JsonDocument().save(to_object(), fs::File(m_is_overwrite, m_path));
    }
  }

  JSON_ACCESS_STRING(Settings, name);
  JSON_ACCESS_STRING(Settings, source);
  JSON_ACCESS_ARRAY(Settings, Font, fonts);
  JSON_ACCESS_ARRAY(Settings, Asset, assets);
  JSON_ACCESS_ARRAY(Settings, Theme, themes);
  JSON_ACCESS_OBJECT(Settings, Icons, icons);
  JSON_ACCESS_BOOL_WITH_KEY(Settings, fontDirty, font_dirty);
  JSON_ACCESS_BOOL_WITH_KEY(Settings, assetsDirty, assets_dirty);

  var::PathString get_output_directory() const {
    return get_source() / "designlab";
  }

  void update_dirty_bits(const var::StringView form_name) {
    if (form_name == fonts_key()) {
      set_font_dirty(true);
    } else if (form_name == assets_key()) {
      set_assets_dirty(true);
    }
  }

private:
  API_AC(Settings, var::KeyString, bad_key);
  var::PathString m_path;
  IsOverwrite m_is_overwrite = IsOverwrite::no;
};

#endif // DESIGNLAB_LOGIC_SETTINGS_HPP
