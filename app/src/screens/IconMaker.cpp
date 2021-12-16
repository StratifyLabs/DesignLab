//
// Created by Tyler Gilbert on 12/12/21.
//

#include <lvgl.hpp>
#include <var.hpp>

#include "IconMaker.hpp"
#include "extras/Extras.hpp"

void IconMaker::configure(Generic generic) {
  generic.clear_flag(Flags::scrollable)
    .add(Container()
           .clear_flag(Flags::scrollable)
           .fill()
           .add(Column(Maker::Names::input_form_column)
                  .fill()
                  .clear_flag(Flags::scrollable)
                  .setup([](Column column) {
                    column.add(ScreenHeading("Icons")).add(HorizontalLine());
                    column.add(Row(Names::control_row).fill_width());
                    column.add(Container()
                                 .fill_width()
                                 .set_flex_grow()
                                 .add_flag(Flags::scrollable)
                                 .add(IconGrid(Names::icon_grid)));
                    add_icons(column.find<IconGrid>(Names::icon_grid));
                    // column.add(Form().setup(configure_form_input));
                  })));

  configure_control_row(generic.find<Row>(Names::control_row));
}

void IconMaker::configure_control_row(Row row) {

  row.add(Button()
            .add_label(StringView(Icons::plus) & " Select All")
            .add_event_callback(EventCode::clicked, select_all));

  row.add(Button()
            .add_label(StringView(Icons::plus) & " Deselect All")
            .add_event_callback(EventCode::clicked, deselect_all));
}

void IconMaker::select_all(lv_event_t *) {
  auto grid = screen().find<IconGrid>(Names::icon_grid);
  for(auto icon: grid){
    icon.get<IconCheck>().set_checked(true);
  }
}
void IconMaker::deselect_all(lv_event_t *) {
  auto grid = screen().find<IconGrid>(Names::icon_grid);
  for(auto icon: grid){
    icon.get<IconCheck>().set_checked(false);
  }
}

void IconMaker::add_icons(IconGrid icon_grid) {
  for (const auto i : api::Index(7 * 35)) {
    icon_grid.add(IconCheck(Icons::file, "file", "uf15b"));
  }
  icon_grid.add(IconCheck(Icons::chevron_right, "file", "ucccc"));
}

