#include <design.hpp>
#include <lvgl.hpp>

#include "Application.hpp"

#include "designlab/fonts/fonts.h"
#include "designlab/themes/themes.h"

#include "view/Home.hpp"
#include "view/ThemePreview.hpp"

#include <design/macros.hpp>

INCBIN(assetfs, "../app/src/designlab/assets/assets.assetfs");
INCBIN_EXTERN(assetfs);

void Application::run(sys::Cli &cli) {

  lvgl::Runtime runtime(
    "Design Lab",
    window::Point(),
    window::Size(320 * 6, 240 * 6),
    window::Window::Flags::shown | window::Window::Flags::highdpi
      | window::Window::Flags::resizeable);

  runtime.window().set_minimum_size(window::Size(800, 600));

  // make the fonts available to `Font::find()`
  fonts_initialize();
  themes_initialize();

  // mount the assets FS which include the PNG icon
  // this file is distributed with the binary rather than as a separate file
  static lv_fs_drv_t drive;
  lvgl_api_mount_asset_filesystem(DESIGN_INCBIN_DATA(assetfs), &drive, 'a');

  // load the PNG decoder
  lvgl_api_initialize_png_decoder();
  {
    auto model = ModelInScope();
    model.instance.runtime = &runtime;
    model.instance.is_export_on_startup = (cli.get_option("export") == "true");
    model.instance.light_theme = Theme::find("default-light-medium");
    model.instance.dark_theme = Theme::find("default-dark-medium");

    if (fs::FileSystem().directory_exists(
          model.instance.session_settings.get_project())) {
      model.instance.is_project_path_valid = true;
    }

    auto session_theme = model.instance.session_settings.get_theme();
    model.instance.is_dark_theme = session_theme == "dark";

    Display(runtime.display())
      .set_theme(
        model.instance.is_dark_theme ? model.instance.dark_theme
                                     : model.instance.light_theme);

    runtime.window().set_position(window::Point(
      model.instance.session_settings.get_window_x(),
      model.instance.session_settings.get_window_y()));
  }

  Home::configure(screen().get<Generic>());
  runtime.loop();

  const auto final_position = runtime.window().get_position();
  ModelInScope()
    .instance.session_settings.set_window_x(final_position.x())
    .set_window_y(final_position.y());
}
