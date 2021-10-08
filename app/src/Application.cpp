#include <lvgl.hpp>

#include "Application.hpp"
#include "Icons.hpp"
#include "Properties.hpp"
#include "Tools.hpp"
#include "Tree.hpp"

Group Elements::keyboard_group;

Application::Application(lvgl::Group keyboard_group) {
  static auto screen = Container::active_screen();

  model().title_font = Font::find(72, Font::Style::semi_bold).get_font();

  model()
    .column_flow_style.set_flex_layout()
    .set_flex_flow(FlexFlow::column)
    .set_flex_align(SetFlexAlign().set_main(FlexAlign::start));

  model().fill_parent_style.set_width(100_percent).set_height(100_percent);

  model().selected_object = screen.object();

  private_model().keyboard_group = keyboard_group;
  Elements::keyboard_group = private_model().keyboard_group;

  static const lv_coord_t tools_width = screen.get_width() / 6;
  static const lv_coord_t properties_width = screen.get_width() / 4;
  static const lv_coord_t canvas_width
    = screen.get_width() - tools_width - properties_width - 20;

  screen.set_padding(15)
    .clear_flag(Container::Flags::scrollable)
    .add(Container(tools_container_name)
           .configure([](Container &container) {
             container.set_width(tools_width)
               .set_height(100_percent)
               .set_background_color(
                 Color::get_palette(Palette::grey, PaletteLevel::darken_x2))
               .add_style(model().column_flow_style);
             Tools::configure(container);
           }))
    .add(Container(canvas_container_name)
           .configure([](Container &container) {
             container.set_width(canvas_width)
               .set_x(tools_width)
               .set_height(100_percent)
               .set_background_color(
                 Color::get_palette(Palette::grey, PaletteLevel::lighten_x4));
           }))
    .add(
      TabView(right_tab_view_name, TabView::Construct(Direction::top, 80))
        .configure([](TabView &tab_view) {
          tab_view.set_width(properties_width)
            .set_x(tools_width + canvas_width)
            .set_height(100_percent)
            .add_tab(
              properties_container_name,
              "Properties",
              [](Container &container) {
                container.add_style(model().fill_parent_style)
                  .set_background_color(
                    Color::get_palette(Palette::grey, PaletteLevel::darken_x2))
                  .add_style(model().column_flow_style);
                Properties::configure(container);
              })
            .add_tab(tree_container_name, "Tree", [](Container &container) {
              container.add_style(model().fill_parent_style)
                .set_background_color(
                  Color::get_palette(Palette::grey, PaletteLevel::darken_x2))
                .add_style(model().column_flow_style);
              Tree::configure(container);
            });
        }));

  private_model().periodic_timer
    = lvgl::PeriodicTimer("applicationTimer", 20_milliseconds, handle_periodic);
}

void Application::handle_periodic(lv_timer_t *) {}
