//
// Created by Tyler Gilbert on 12/8/21.
//

#include "ThemeMaker.hpp"
#include "ThemePreview.hpp"
#include "IconMaker.hpp"
#include "FontMaker.hpp"
#include "AssetMaker.hpp"

#include "Home.hpp"

void Home::configure(Generic generic) {

  generic.add(
    Row()
      .fill()
      .set_padding(0)
      .add(Container()
             .add_style("darker")
             .set_width(450)
             .fill_height()
             .set_border_side(BorderSide::right)
             .set_border_width(3)
             .add(Column().setup(configure_button_column)))
      .add(NakedContainer(Names::content_area).set_flex_grow().fill_height()));
}

void Home::configure_button_column(Column column) {
  column.add_style("darker").fill().add(SectionHeading("Design Lab"));

  add_side_button(column, "", "Theme", ThemeMaker::configure);
  add_side_button(column, "", "Assets", AssetMaker::configure);
  add_side_button(column, "", "Fonts", FontMaker::configure);
  add_side_button(column, "", "Icons", IconMaker::configure);
  add_side_button(column, "", "Export", nullptr);

  column.add(NakedContainer().set_flex_grow());
  add_side_button(column, "", "Theme Preview", ThemePreview::configure);
  column.add(Label().set_text_static("Account"));
}

void Home::add_side_button(
  Column column,
  const char *icon,
  const char *name,
  void (*callback)(Generic)) {
  column.add(Button(name)
               .add_flag(Flags::checkable)
               .clear_state(State::checked)
               .fill_width()
               .add_static_label(name)
               .add_event_callback(
                 EventCode::clicked,
                 (void *)callback,
                 [](lv_event_t *e) {
                   const auto event = Event(e);
                   const auto callback = event.user_data<void (*)(Generic)>();
                   auto self = event.target<Button>();
                   // mark as not checked
                   auto button_area = self.get_parent().get<Generic>();
                   for (auto i : api::Index(button_area.get_child_count())) {
                     auto current = button_area.get_child(i).get<Generic>();
                     if (current.object() != self.object()) {
                       button_area.get_child(i).get<Generic>().clear_state(
                         State::checked);
                     }
                   }

                   if (callback) {
                     if (self.has_state(State::checked)) {
                       callback(
                         screen().find<Generic>(Names::content_area).clean());
                     } else {
                       self.add_state(State::checked);
                     }
                   }
                 }));
};
