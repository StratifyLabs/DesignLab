//
// Created by Tyler Gilbert on 12/16/21.
//

#ifndef DESIGNLAB_FONTMANAGER_HPP
#define DESIGNLAB_FONTMANAGER_HPP

#include "Manager.hpp"

class FontManager : public Manager {
public:
  using UpdateCallback = void (*)(void*,int,int);

  struct Construct {
    API_PMAZ(icons, Construct, Settings::Icons, {});
    API_PMAZ(input_path, Construct, var::PathString, {});

    API_PUBLIC_BOOL(Construct,dry_run,false);

    API_PMAZ(lv_font_conv_path, Construct, var::PathString, {});
    API_PMAZ(output_path, Construct, var::PathString, {});
    API_PMAZ(project_path, Construct, var::PathString, {});
    API_PMAZ(update_callback, Construct, UpdateCallback, nullptr);
    API_PMAZ(update_context, Construct, void*, nullptr);

  };

  FontManager(const Construct &options);

  static var::GeneralString get_size_list(const Settings::Font &font) {
    return font.get_sizes();
  }

  static var::PathString
  get_file_name(const Settings::Font &font, const var::StringView size) {
    return font.get_name() & "_"
           & lvgl::Font::to_abbreviated_cstring(
             lvgl::Font::style_from_string(font.get_style()))
           & "_" & size & ".c";
  }

  fs::PathList get_source_list(
    var::StringView project_path,
    const Settings &settings) override;

  template <class Context>
  void for_each_font(
    const var::Vector<Settings::Font> &font_list,
    Context &context,
    void (*callback)(const Settings::Font &, var::StringView, Context &)) {
    for (const auto &font : font_list) {
      const auto sizes = get_size_list(font);
      for (const auto font_size : sizes.string_view().split(",")) {
        callback(font, font_size, context);
      }
    }
  }

private:

  Settings::Icons m_icons;
  var::PathString m_lv_font_conv_path;

  var::String
  get_icon_font_range(Settings::Icons icons, var::StringView family);
  void generate_bootstrap_icons_hpp(var::StringView directory);
  void generate_fontawesome_icons_hpp(var::StringView directory);
  void generate_fonts_source(
    var::StringView directory,
    const var::Vector<Settings::Font> &font_list);

  var::PathString get_temporary_font_path(var::StringView filename);
};

#endif // DESIGNLAB_FONTMANAGER_HPP
