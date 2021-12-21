//
// Created by Tyler Gilbert on 12/14/21.
//

#include "Extras.hpp"

#include "designlab/fonts/FontAwesomeIcons.hpp"

#include <design.hpp>

HeaderRow::HeaderRow(
  const char *title,
  const char *button_icon,
  const char *button_text,
  Event::Callback callback,
  const char * color) {
  construct_object(title);
  add_style(Row::get_style())
    .add_style("row")
    .fill_width()
    .add(ScreenHeading(title).set_flex_grow())
    .add(
      Button(Names::action_button)
        .add_style(color)
        .add_label(var::KeyString(button_icon).append(" ").append(button_text))
        .add_event_callback(EventCode::clicked, callback));
}

FormHeaderRow::FormHeaderRow(const char *title, Event::Callback callback) {
  construct_object(title);
  add_style(Row::get_style())
    .add_style("row")
    .fill_width()
    .add(Button()
           .add_label_as_static(icons::fa::times_solid)
           .add_style("btn_outline_primary circle")
           .add_event_callback(EventCode::clicked, callback))
    .add(
      ScreenHeading(title).set_width(Row::size_from_content).set_flex_grow());
}

AddButtonRow::AddButtonRow(
  const char *button_name,
  const char *icon,
  const char *title,
  Event::Callback callback) {
  construct_object(title);
  add_style(Row::get_style())
    .add_style("row")
    .fill_width()
    .set_height(Row::size_from_content)
    .add(Button(button_name)
           .add_style("btn_primary")
           .add_label(var::KeyString(icon).append(" ").append(title))
           .add_event_callback(EventCode::clicked, callback));
}

InfoCard InfoCard::get_from_member(lvgl::Object object) {
  auto card = object.find_parent<Card>(Names::card);
  return InfoCard(card.get_parent().object());
}

InfoCard::Data *InfoCard::get_data_from_event(lv_event_t *e) {
  return get_from_member(Event(e).target()).user_data<Data>();
}

InfoCard::InfoCard(Data &data) {
  construct_object(data.cast_as_name());
  fill_width();
  set_height(size_from_content);
  add(
    Card(Names::card)
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
                    .add_style("list_group_flush")))
      .add(
        Row(Names::button_row)
          .add_style("card_footer")
          .fill_width()
          .set_height(size_from_content)
          .add(Button(Names::edit_button)
                 .add_style(Row::get_style())
                 .set_column_padding(12)
                 .set_padding(16)
                 .add_label(icons::fa::edit_solid)
                 .add_label("Edit")
                 .add_style("btn_outline_primary btn_md")
                 .add_event_callback(EventCode::clicked, data.edit_callback))
          .add(
            Button(Names::remove_button)
              .add_style(Row::get_style())
              .set_column_padding(12)
              .set_padding(16)
              .add_label(icons::fa::trash_alt_solid)
              .add_label("Delete")
              .add_style("btn_outline_danger btn_md")
              .add_event_callback(EventCode::clicked, data.remove_callback))));

  if (data.edit_callback == nullptr) {
    find<Button>(Names::edit_button).add_flag(Flags::hidden);
  }

  if (data.remove_callback == nullptr) {
    find<Button>(Names::remove_button).add_flag(Flags::hidden);
  }

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

IconCheck::IconCheck(u16 icon_unicode, const var::StringView name, const var::StringView family, Font font) {
  construct_object("");
  add_style(Column::get_style())
    .add_style("col")
    .set_width(24_percent)
    .set_height(size_from_content)
    .add(Button(Names::checkable_button)
           .add(Label()
                  .set_text_font(font)
                  .set_text(Font::Utf8Character(icon_unicode).cstring()))
           .add_style("btn_light btn_lg")
           .add_flag(Flags::checkable))
    .add(Label(Names::name_label).set_text(var::KeyString(name)))
    .add(Label(Names::family_label).set_text(var::KeyString(family)))
    .add(Label(Names::unicode_label)
           .set_text(var::NumberString(icon_unicode, "u%04x")));
}

const char *IconCheck::get_name() const {
  return find<Label>(Names::name_label).get_text();
}

const char *IconCheck::get_family() const {
  return find<Label>(Names::family_label).get_text();
}

IconCheck &IconCheck::set_checked(bool value) {
  auto button = find<lvgl::Button>(Names::checkable_button);
  if (value) {
    button.add_state(State::checked);
  } else {
    button.clear_state(State::checked);
  }
  return *this;
}

const char *IconCheck::get_unicode() const {
  return find<Label>(Names::unicode_label).get_text();
}

bool IconCheck::is_checked() const {
  auto button = find<lvgl::Button>(Names::checkable_button);
  return button.has_state(State::checked);
}

IconGrid::IconGrid(const char *name) {
  construct_object(name);
  clear_flag(Flags::scrollable)
    .fill_width()
    .set_height(Container::size_from_content)
    .set_column_padding(1)
    .set_row_padding(50)
    .set_flex_layout()
    .set_flex_flow(lvgl::FlexFlow::row_wrap)
    .set_flex_align(SetFlexAlign()
                      .set_main(FlexAlign::space_between)
                      .set_track_cross(FlexAlign::start));
}

IconGridContainer::IconGridContainer(const char *name) {
  construct_object(name);
  fill()
    .add_flag(Flags::scrollable)
    .set_scroll_direction(Direction::vertical)
    .add(IconGrid(Names::icon_grid));
}