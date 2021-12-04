#include <design.hpp>
#include <lvgl.hpp>

#include "Application.hpp"
#include "Icons.hpp"
#include "Properties.hpp"
#include "Tools.hpp"
#include "Tree.hpp"

#include "fonts/fonts.h"
#include "themes/themes.h"

extern "C" const char data_assetfs[];

void Application::run(sys::Cli &cli) {

  lvgl::Runtime runtime(
    "gui",
    window::Point(),
    window::Size(320 * 4, 240 * 4),
    window::Window::Flags::shown | window::Window::Flags::highdpi
      | window::Window::Flags::resizeable);

  runtime.window().set_minimum_size(window::Size(480, 360));

  // make the fonts available to `Font::find()`
  fonts_initialize();

  // mount the assets FS which include the PNG icon
  // this file is distributed with the binary rather than as a separate file
  static lv_fs_drv_t drive;
  lvgl_api_mount_asset_filesystem(data_assetfs, &drive, 'd');
  // Icon is at d:icon256x256.png

  // load the PNG decoder
  lvgl_api_initialize_png_decoder();

  auto theme
    = Theme(lvgl_small_dark_theme_initialize(runtime.display(), nullptr));

  printf("Theme is at %p\n", theme.native_value());

  API_PRINTF_TRACE_LINE();
  lv_disp_set_theme(runtime.display(), theme.native_value());
  API_PRINTF_TRACE_LINE();
  static const auto s = Style().set_background_color(Color::red());

  screen()
    .clear_flag(Flags::scrollable)
    .add(Row()
           .fill()
           .set_padding(20)
           .set_column_padding(20)
           .add(Column()
                  .set_padding(20)
                  .set_width(50_percent)
                  .fill_height()
                  .set_row_padding(20)
                  .add(Button("HelloButton")
                         .add_style(s)
                         .set_width(100)
                         .set_height(20)
                         .set_background_color(Color::blue())
                         .add_label("Hello")))
           .add(Column()
                  .set_padding(20)
                  .set_width(50_percent)
                  .fill_height()
                  .set_row_padding(20)
                  .add(Button().add_label("World")))
           .add(Column()));

  auto button = screen().find<Button>("HelloButton");

  button.update_layout();

  printf("screen has %d styles\n", screen().object()->style_cnt);

  printf("Button has %d styles\n", button.object()->style_cnt);
  printf(
    "bg opacity is %d\n",
    button.get_property_value(Property::background_opacity).number());
  printf(
    "width is %d - %d\n",
    button.get_property_value(Property::width).number(),
    button.get_width());

  // model cannot be touched until all lvgl initialization is complete
  // it is initialized on first access
  model().runtime = &runtime;

  runtime.loop();
}

void Application::handle_periodic(lv_timer_t *) {}
