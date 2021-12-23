//
// Created by Tyler Gilbert on 12/16/21.
//

#ifndef DESIGNLAB_FONTMANAGER_HPP
#define DESIGNLAB_FONTMANAGER_HPP

#include "Manager.hpp"

class FontManager : public Manager {
public:
  struct Construct {
    API_PMAZ(input_path, Construct, var::PathString, {});

    API_PUBLIC_BOOL(Construct,dry_run,false);

    API_PMAZ(output_path, Construct, var::PathString, {});
    API_PMAZ(project_path, Construct, var::PathString, {});

  };

  FontManager(const Construct &options);

  static var::GeneralString get_size_list(const Settings::Font &font) {
    auto start = font.get_sizes_start().to_integer();
    const auto step_list = font.get_sizes_steps().split(",");
    const auto total = font.get_sizes_total().to_integer();
    var::GeneralString result;

    int step = 0;
    for (const auto i : api::Index(total)) {
      result.append(var::NumberString(start, "%d,").string_view());
      if (i < step_list.count()) {
        step = step_list.at(i).to_integer();
      }
      start += step;
    }
    return result.pop_back();
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
