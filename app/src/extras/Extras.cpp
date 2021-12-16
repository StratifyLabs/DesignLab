//
// Created by Tyler Gilbert on 12/14/21.
//

#include "Extras.hpp"

#include "designlab/fonts/Icons.hpp"

#include <design.hpp>

HeaderRow::HeaderRow(
  const char *title,
  const char *button_icon,
  const char *button_text,
  Event::Callback callback) {
  construct_object(title);
  add_style(Row::get_style())
    .add_style("row")
    .fill_width()
    .add(ScreenHeading(title).set_flex_grow())
    .add(
      Button()
        .add_style("btn_outline_primary")
        .add_label(var::KeyString(button_icon).append(" ").append(button_text))
        .add_event_callback(EventCode::clicked, callback));
}

FormHeaderRow::FormHeaderRow(const char *title, Event::Callback callback) {
  construct_object(title);
  add_style(Row::get_style())
    .add_style("row")
    .fill_width()
    .add(Button()
           .add_label_as_static(Icons::times)
           .add_style("btn_outline_primary")
           .add_event_callback(EventCode::clicked, callback))
    .add(
      ScreenHeading(title).set_width(Row::size_from_content).set_flex_grow());
}

AddButtonRow::AddButtonRow(const char *title, Event::Callback callback) {
  construct_object(title);
  add_style(Row::get_style())
    .add_style("row")
    .fill_width()
    .set_height(Row::size_from_content)
    .add(Button()
           .add_style("btn_primary")
           .add_label(var::KeyString(Icons::plus).append(" ").append(title))
           .add_event_callback(EventCode::clicked, callback));
}

InfoCard::InfoCard(Data &data) {
  construct_object(data.cast_as_name());
  fill_width();
  set_height(size_from_content);
  add(Card(Names::card)
        .fill_width()
        .add_style(Column::get_style())
        .set_height(size_from_content)
        .add(Card::Header(data.title.cstring()))
        .add(Row(Names::card_body_row)
               .add_style("card_body")
               .fill_width()
               .justify_space_around()
               .set_height(size_from_content)
               .add(List(Names::card_body_left_column)
                      .fill_width()
                      .add_style("list_group_flush"))));

  // add half the features on the left and half on the right
  auto left_list = find<List>(Names::card_body_left_column);
  const auto feature_count = data.feature_list().count();
  for (const auto i : api::Index(feature_count)) {
    const auto &feature = data.feature_list().at(i);
    auto &list = left_list;
    list.add_button(
      feature.value.cstring(),
      feature.icon,
      feature.label,
      [](Button button) {
        button.add(Label().set_text_as_static(button.name()));
      });
  }
}

IconCheck::IconCheck(const char *icon, const char *name, const char *unicode) {
  construct_object(name);
  const auto *icon_unsigned = reinterpret_cast<const u8 *>(icon);
  add_style(Column::get_style())
    .add_style("col")
    .set_height(size_from_content)
    .add(Button(Names::checkable_button)
           .add_label_as_static(icon)
           .add_style("btn_light btn_lg")
           .add_flag(Flags::checkable))
    .add(
      Label().set_text(var::NumberString().format("%s - %s", name, unicode)));
}

IconGrid::IconGrid(const char *name) {
  construct_object(name);
  add_flag(Flags::scrollable)
    .fill_width()
    .set_height(Container::size_from_content)
    .set_column_padding(50)
    .set_row_padding(50)
    .set_flex_layout()
    .set_flex_flow(lvgl::FlexFlow::row_wrap)
    .set_flex_align(SetFlexAlign()
                      .set_main(FlexAlign::space_around)
                      .set_track_cross(FlexAlign::start));
}