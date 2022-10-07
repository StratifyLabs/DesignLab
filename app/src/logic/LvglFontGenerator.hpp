//
// Created by Tyler Gilbert on 10/6/22.
//

#ifndef DESIGNLAB_LVGLFONTGENERATOR_HPP
#define DESIGNLAB_LVGLFONTGENERATOR_HPP

#include <var/StringView.hpp>

#include <lvgl/Font.hpp>

class LvglFontGenerator {
public:

  struct Construct {
    var::StringView name;
    u16 size;
    lvgl::Font::Style style;
  };

  LvglFontGenerator(const Construct & options);

  LvglFontGenerator & export_as_c_source_file(var::StringView output_path);

private:
  static void deleter(lv_font_t * font);
  using Resource = api::SystemResource<lv_font_t, decltype(&deleter)>;

  Resource m_resource;
};

#endif // DESIGNLAB_LVGLFONTGENERATOR_HPP
