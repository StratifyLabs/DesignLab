#include <design.hpp>
#include <lvgl.hpp>

#include "Application.hpp"

#include "designlab/fonts/fonts.h"
#include "designlab/themes/themes.h"

#include "screens/Home.hpp"
#include "screens/ThemePreview.hpp"

extern "C" const char data_assetfs[];

void Application::run(sys::Cli &cli) {

  lvgl::Runtime runtime(
    "Design Lab",
    window::Point(),
    window::Size(320 * 6, 240 * 6),
    window::Window::Flags::shown | window::Window::Flags::highdpi
      /*| window::Window::Flags::resizeable*/);

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

  model().runtime = &runtime;
  model().theme
    = Theme(lvgl_small_light_theme_initialize(runtime.display(), nullptr));

  lv_disp_set_theme(runtime.display(), model().theme.native_value());
  static const auto s = Style().set_background_color(Color::red());

  Screen theme_preview_screen(ThemePreview::screen_name);
  ThemePreview::configure(theme_preview_screen.get<Generic>());

  // theme_preview_screen.load();

  Home::configure(screen().get<Generic>());

  // screen().add(Container().fill());

  // model cannot be touched until all lvgl initialization is complete
  // it is initialized on first access

  runtime.loop();
}

void Application::handle_periodic(lv_timer_t *) {}
