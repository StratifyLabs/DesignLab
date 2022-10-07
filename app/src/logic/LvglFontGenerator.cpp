//
// Created by Tyler Gilbert on 10/6/22.
//

#include <var/String.hpp>

#include "LvglFontGenerator.hpp"

#include <lv_freetype.h>

using namespace lvgl;
using namespace var;

namespace {

auto get_freetype_style(Font::Style style) -> u16 {
  switch (style) {
  case Font::Style::extra_light:
  case Font::Style::thin:
  case Font::Style::light:
  case Font::Style::regular:
  case Font::Style::medium:
    return FT_FONT_STYLE_NORMAL;
  case Font::Style::thin_italic:
  case Font::Style::extra_light_italic:
  case Font::Style::light_italic:
  case Font::Style::regular_italic:
  case Font::Style::medium_italic:
    return FT_FONT_STYLE_ITALIC;
  case Font::Style::semi_bold_italic:
  case Font::Style::bold_italic:
  case Font::Style::extra_bold_italic:
    return FT_FONT_STYLE_ITALIC | FT_FONT_STYLE_BOLD;
  case Font::Style::semi_bold:
  case Font::Style::bold:
  case Font::Style::extra_bold:
    return FT_FONT_STYLE_BOLD;
  default:
    break;
  }
  return FT_FONT_STYLE_NORMAL;
}
} // namespace

LvglFontGenerator::LvglFontGenerator(
  const LvglFontGenerator::Construct &options) {
  lv_freetype_init(0, 0, 0);

  const auto name = String{options.name};

  auto font_info = lv_ft_info_t{
    .name = name.cstring(),
    .font = m_resource.pointer_to_value(),
    .weight = options.size,
    .style = get_freetype_style(options.style)};

  const auto result = lv_ft_font_init(&font_info);
  if (!result) {
    API_RETURN_ASSIGN_ERROR("failed to load font " | options.name, EINVAL);
  }
}

void LvglFontGenerator::deleter(lv_font_t *font) {
  lv_ft_font_destroy(font);
  lv_freetype_destroy();
}

LvglFontGenerator &
LvglFontGenerator::export_as_c_source_file(var::StringView output_path) {

  printf("export file\n");
  return *this;
}
