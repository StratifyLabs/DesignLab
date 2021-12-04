#include <lvgl.hpp>

#include "Application.hpp"
#include "Icons.hpp"
#include "Properties.hpp"
#include "Tools.hpp"
#include "Tree.hpp"

#include "fonts/fonts.h"
extern "C" const char data_assetfs[];

void Application::run(sys::Cli &cli) {

  lvgl::Runtime runtime(
    "gui",
    window::Point(),
    window::Size(320 * 4, 240 * 4),
    window::Window::Flags::shown | window::Window::Flags::highdpi
      | window::Window::Flags::borderless);

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

  // model cannot be touched until all lvgl initialization is complete
  // it is initialized on first access
  model().runtime = &runtime;

  static auto screen = Container::active_screen();

  model().title_font = Font::find(72, Font::Style::semi_bold).get_font();

  model()
    .column_flow_style.set_flex_layout()
    .set_flex_flow(FlexFlow::column)
    .set_flex_align(SetFlexAlign().set_main(FlexAlign::start));

  model().fill_parent_style.set_width(100_percent).set_height(100_percent);

  model().selected_object = screen.object();

  static const lv_coord_t tools_width = screen.get_width() / 6;
  static const lv_coord_t properties_width = screen.get_width() / 4;
  static const lv_coord_t canvas_width
    = screen.get_width() - tools_width - properties_width - 20;

  screen.set_padding(15)
    .clear_flag(Container::Flags::scrollable)
    .add(Container(tools_container_name).setup([](Container container) {
      container.set_width(tools_width)
        .set_height(100_percent)
        .set_background_color(
          Color::get_palette(Palette::grey, PaletteLevel::darken_x2))
        .add_style(model().column_flow_style);
      Tools::configure(container);
    }))
    .add(Container(canvas_container_name).setup([](Container container) {
      container.set_width(canvas_width)
        .set_x(tools_width)
        .set_height(100_percent)
        .set_background_color(
          Color::get_palette(Palette::grey, PaletteLevel::lighten_x4));
    }))
    .add(
      TabView(TabView::Construct()
                .set_name(right_tab_view_name)
                .set_direction(Direction::top)
                .set_size(80))
        .setup([](TabView tab_view) {
          tab_view.set_width(properties_width)
            .set_x(tools_width + canvas_width)
            .set_height(100_percent)
            .add_tab(
              properties_container_name,
              "Properties",
              [](Container container) {
                container.add_style(model().fill_parent_style)
                  .set_background_color(
                    Color::get_palette(Palette::grey, PaletteLevel::darken_x2))
                  .add_style(model().column_flow_style);
                Properties::configure(container);
              })
            .add_tab(tree_container_name, "Tree", [](Container container) {
              container.add_style(model().fill_parent_style)
                .set_background_color(
                  Color::get_palette(Palette::grey, PaletteLevel::darken_x2))
                .add_style(model().column_flow_style);
              Tree::configure(container);
            });
        }));

  private_model().periodic_timer
    = lvgl::PeriodicTimer("applicationTimer", 20_milliseconds, handle_periodic);

  runtime.loop();
}

void Application::handle_periodic(lv_timer_t *) {}
