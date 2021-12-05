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

  model().runtime = &runtime;

  model().theme
    = Theme(lvgl_small_dark_theme_initialize(runtime.display(), nullptr));

  lv_disp_set_theme(runtime.display(), model().theme.native_value());
  static const auto s = Style().set_background_color(Color::red());

  screen()
    .clear_flag(Flags::scrollable)
    .add(
      Row()
        .fill()
        .set_padding(20)
        .set_column_padding(20)
        .add(Column()
               .set_padding(20)
               .set_width(50_percent)
               .fill_height()
               .set_row_padding(20)
               .add(Button().add_style("primary sm").add_label("Primary"))
               .add(Button().add_style("secondary").add_label("Secondary"))
               .add(Button().add_style("info").add_label("Info"))
               .add(Button().add_style("warning").add_style("pill").add_label("Warning"))
               .add(Button().add_style("danger lg pill").add_label("Danger"))
               .add(Button().add_style("success").add_style("sm").add_label("Success")))
        .add(Column()
               .set_padding(20)
               .set_width(50_percent)
               .fill_height()
               .set_row_padding(20)
               .add(Button().add_style("outline_primary").add_label("Outline Primary"))
               .add(Button().add_style("outline_secondary").add_label("Outline Secondary"))
               .add(Button().add_style("outline_info").add_label("Outline Info"))
               .add(Button().add_style("outline_warning").add_label("Outline Warning"))
               .add(Button().add_style("outline_danger").add_label("Outline Danger"))
               .add(Button().add_style("outline_success").add_label("Outline Success")))
        .add(Column()));


  // model cannot be touched until all lvgl initialization is complete
  // it is initialized on first access

  runtime.loop();
}

void Application::handle_periodic(lv_timer_t *) {}
