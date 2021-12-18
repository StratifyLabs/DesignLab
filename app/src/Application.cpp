#include <design.hpp>
#include <lvgl.hpp>

#include "Application.hpp"

#include "designlab/fonts/fonts.h"
#include "designlab/themes/themes.h"

#include "view/Home.hpp"
#include "view/ThemePreview.hpp"

extern "C" const char assets[];

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
  lvgl_api_mount_asset_filesystem(assets, &drive, 'a');
  // Icon is at a:icon256x256.png

  // load the PNG decoder
  lvgl_api_initialize_png_decoder();

  {
    Model::Scope model_scope;
    model().runtime = &runtime;
    model().light_theme
      = Theme(lvgl_small_light_theme_initialize(runtime.display(), nullptr));
    model().dark_theme
      = Theme(lvgl_small_dark_theme_initialize(runtime.display(), nullptr));

    Display(runtime.display()).set_theme(model().light_theme);
  }

  Home::configure(screen().get<Generic>());
  runtime.loop();
}
