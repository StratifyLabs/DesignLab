#include <lvgl.h>
#include <lvgl_api.h>

extern const lv_font_t montserrat_r_8;
extern const lv_font_t montserrat_r_12;
extern const lv_font_t montserrat_r_16;
extern const lv_font_t montserrat_r_20;
extern const lv_font_t montserrat_r_24;
extern const lv_font_t montserrat_r_28;
extern const lv_font_t montserrat_r_44;
extern const lv_font_t montserrat_r_56;
extern const lv_font_t montserrat_r_72;
extern const lv_font_t montserrat_i_8;
extern const lv_font_t montserrat_i_12;
extern const lv_font_t montserrat_i_16;
extern const lv_font_t montserrat_i_20;
extern const lv_font_t montserrat_i_24;
extern const lv_font_t montserrat_i_28;
extern const lv_font_t montserrat_i_44;
extern const lv_font_t montserrat_i_56;
extern const lv_font_t montserrat_i_72;
extern const lv_font_t montserrat_l_12;
extern const lv_font_t montserrat_l_8;
extern const lv_font_t montserrat_l_16;
extern const lv_font_t montserrat_l_20;
extern const lv_font_t montserrat_l_24;
extern const lv_font_t montserrat_l_28;
extern const lv_font_t montserrat_l_44;
extern const lv_font_t montserrat_l_56;
extern const lv_font_t montserrat_l_72;
extern const lv_font_t montserrat_sb_8;
extern const lv_font_t montserrat_sb_12;
extern const lv_font_t montserrat_sb_16;
extern const lv_font_t montserrat_sb_20;
extern const lv_font_t montserrat_sb_24;
extern const lv_font_t montserrat_sb_28;
extern const lv_font_t montserrat_sb_44;
extern const lv_font_t montserrat_sb_56;
extern const lv_font_t montserrat_sb_72;
extern const lv_font_t sourcecode_r_8;
extern const lv_font_t sourcecode_r_12;
extern const lv_font_t sourcecode_r_16;
extern const lv_font_t sourcecode_r_20;
extern const lv_font_t sourcecode_r_24;
extern const lv_font_t sourcecode_r_28;
extern const lv_font_t sourcecode_r_44;
extern const lv_font_t sourcecode_r_56;
extern const lv_font_t sourcecode_r_72;

static const lvgl_api_font_descriptor_t lvgl_font_list[] = {
  {.name = "montserrat-r-8", .font = &montserrat_r_8},
  {.name = "montserrat-r-12", .font = &montserrat_r_12},
  {.name = "montserrat-r-16", .font = &montserrat_r_16},
  {.name = "montserrat-r-20", .font = &montserrat_r_20},
  {.name = "montserrat-r-24", .font = &montserrat_r_24},
  {.name = "montserrat-r-28", .font = &montserrat_r_28},
  {.name = "montserrat-r-44", .font = &montserrat_r_44},
  {.name = "montserrat-r-56", .font = &montserrat_r_56},
  {.name = "montserrat-r-72", .font = &montserrat_r_72},
  {.name = "montserrat-i-8", .font = &montserrat_i_8},
  {.name = "montserrat-i-12", .font = &montserrat_i_12},
  {.name = "montserrat-i-16", .font = &montserrat_i_16},
  {.name = "montserrat-i-20", .font = &montserrat_i_20},
  {.name = "montserrat-i-24", .font = &montserrat_i_24},
  {.name = "montserrat-i-28", .font = &montserrat_i_28},
  {.name = "montserrat-i-44", .font = &montserrat_i_44},
  {.name = "montserrat-i-56", .font = &montserrat_i_56},
  {.name = "montserrat-i-72", .font = &montserrat_i_72},
  {.name = "montserrat-l-8", .font = &montserrat_l_8},
  {.name = "montserrat-l-12", .font = &montserrat_l_12},
  {.name = "montserrat-l-16", .font = &montserrat_l_16},
  {.name = "montserrat-l-20", .font = &montserrat_l_20},
  {.name = "montserrat-l-24", .font = &montserrat_l_24},
  {.name = "montserrat-l-28", .font = &montserrat_l_28},
  {.name = "montserrat-l-44", .font = &montserrat_l_44},
  {.name = "montserrat-l-56", .font = &montserrat_l_56},
  {.name = "montserrat-l-72", .font = &montserrat_l_72},
  {.name = "montserrat-sb-8", .font = &montserrat_sb_8},
  {.name = "montserrat-sb-12", .font = &montserrat_sb_12},
  {.name = "montserrat-sb-16", .font = &montserrat_sb_16},
  {.name = "montserrat-sb-20", .font = &montserrat_sb_20},
  {.name = "montserrat-sb-24", .font = &montserrat_sb_24},
  {.name = "montserrat-sb-28", .font = &montserrat_sb_28},
  {.name = "montserrat-sb-44", .font = &montserrat_sb_44},
  {.name = "montserrat-sb-56", .font = &montserrat_sb_56},
  {.name = "montserrat-sb-72", .font = &montserrat_sb_72},
  {.name = "sourcecode-r-8", .font = &sourcecode_r_8},
  {.name = "sourcecode-r-12", .font = &sourcecode_r_12},
  {.name = "sourcecode-r-16", .font = &sourcecode_r_16},
  {.name = "sourcecode-r-20", .font = &sourcecode_r_20},
  {.name = "sourcecode-r-24", .font = &sourcecode_r_24},
  {.name = "sourcecode-r-28", .font = &sourcecode_r_28},
  {.name = "sourcecode-r-44", .font = &sourcecode_r_44},
  {.name = "sourcecode-r-56", .font = &sourcecode_r_56},
  {.name = "sourcecode-r-72", .font = &sourcecode_r_72},

};

static const lvgl_api_font_descriptor_t *get_font(int offset) {
  const int count = sizeof(lvgl_font_list) / sizeof(lvgl_api_font_descriptor_t);
  if (offset >= 0 && offset < count) {
    return lvgl_font_list + offset;
  }
  return NULL;
}

void fonts_initialize() { lvgl_api_set_font_callback(get_font); }
