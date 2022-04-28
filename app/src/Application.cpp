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

  // mount the assets FS which include the PNG icon
  // this file is distributed with the binary rather than as a separate file
  static lv_fs_drv_t drive;
  lvgl_api_mount_asset_filesystem(DESIGN_INCBIN_DATA(assetfs), &drive, 'a');

  // load the PNG decoder
  lvgl_api_initialize_png_decoder();
  {
    Model::Scope model_scope;
    model().runtime = &runtime;
    model().is_export_on_startup = (cli.get_option("export") == "true");
    model().light_theme
      = Theme(default_light_medium_theme_initialize(runtime.display(), nullptr));
    model().dark_theme
      = Theme(default_dark_medium_theme_initialize(runtime.display(), nullptr));

    if (fs::FileSystem().directory_exists(
          model().session_settings.get_project())) {
      model().is_project_path_valid = true;
    }

    auto session_theme = model().session_settings.get_theme();
    model().is_dark_theme = session_theme == "dark";

    Display(runtime.display())
      .set_theme(
        model().is_dark_theme ? model().dark_theme : model().light_theme);

    runtime.window().set_position(window::Point(
      model().session_settings.get_window_x(),
      model().session_settings.get_window_y()));
  }


  Home::configure(screen().get<Generic>());
  runtime.loop();

  {
    Model::Scope model_scope;
    const auto final_position = runtime.window().get_position();
    model().session_settings.set_window_x(final_position.x());
    model().session_settings.set_window_y(final_position.y());
  }
}

