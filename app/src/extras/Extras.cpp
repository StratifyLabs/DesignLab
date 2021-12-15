//
// Created by Tyler Gilbert on 12/14/21.
//

#include "Extras.hpp"

#include "fonts/Icons.hpp"

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
