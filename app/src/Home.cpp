#include <lvgl.hpp>

#include "Home.hpp"
#include "FileBrowser.hpp"
#include "Icons.hpp"

void Home::configure(lvgl::Container &container) {

  static const auto font_info = Font::find(44);

  printer().object("font", font_info);
  container.add(
    TabView(tab_view_name)
      .set_initial_direction(Direction::left)
      .set_initial_size(15_percent)
      .configure([](TabView &tab_view) {
        tab_view.set_text_font(font_info)
          .clear_flag(TabView::Flags::scrollable)
          .add_tab(cloud_tab_name, Icons::cloud, configure_cloud)
          .add_tab(os_tab_name, Icons::microchip, configure_device)
          .add_tab(
            filesystem_tab_name,
            Icons::folder_open,
            configure_filesystem)
          .add_tab(key_tab_name, Icons::lock, configure_keys)
          .add_tab(terminal_tab_name, Icons::terminal, configure_terminal)
          .add_tab(about_tab_name, Icons::info, configure_about)
          .add_tab(exit_tab_name, Icons::times, configure_exit);


        auto buttons = tab_view.get_buttons();
        buttons.set_border_side(
          BorderSide::right,
          Selector(Part::items, State::checked));

        tab_view.get_content().add_event_callback(
          EventCode::scroll_begin,
          [](lv_event_t *e) {
            auto a = Event(e).parameter<lv_anim_t *>();
            if (a) {
              a->time = 0;
            }
          });
      }));
}
