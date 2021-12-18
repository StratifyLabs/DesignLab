//
// Created by Tyler Gilbert on 12/12/21.
//

#include <lvgl.hpp>
#include <var.hpp>

#include "IconMaker.hpp"
#include "extras/Extras.hpp"

void IconMaker::configure(Generic generic) {
  generic.clear_flag(Flags::scrollable)
    .add(Container(Names::first_child_container)
           .clear_flag(Flags::scrollable)
           .fill()
           .add(Column(Names::input_form_column)
                  .fill()
                  .clear_flag(Flags::scrollable)));

  auto input_form_column = generic.find<Column>(Names::input_form_column);

  input_form_column.add(ScreenHeading("Icons"))
    .add(HorizontalLine())
    .add(Row(Names::control_row).fill_width())
    .add(Row(Names::icon_container_row)
           .set_horizontal_padding(0)
           .set_column_padding(0)
           .fill_width()
           .set_flex_grow()
           .set_scroll_direction(Direction::vertical));

  auto bootstrap_icons_list = json::JsonDocument()
                                .load(AssetFile("a:bootstrap-icons.json"))
                                .to_object();
  const auto key_list = bootstrap_icons_list.get_key_list();

  auto add_control_button = [](
                              Row row,
                              const char *name,
                              const char *icon,
                              const char *text,
                              const char *style,
                              Event::Callback callback) {
    row.add(Button(name)
              .add_style(style)
              .add_flag(Flags::checkable)
              .add_style("btn_sm")
              .add_label(StringView(icon) & text)
              .add_event_callback(EventCode::clicked, callback));
  };

  auto control_row = generic.find<Row>(Names::control_row);
  add_control_button(
    control_row,
    Names::select_all_button,
    Icons::plus,
    " Select All",
    "",
    select_all);

  // push the page buttons to the right
  control_row.add(NakedContainer().set_height(1).set_flex_grow());

  auto icon_container_row = generic.find<Row>(Names::icon_container_row);

  const auto icon_count = key_list.count();
  for (u32 offset = 0; offset < icon_count;
       offset += IconGridContainer::max_icon_count) {

    const auto child_index = offset / IconGridContainer::max_icon_count;

    add_control_button(
      control_row,
      "",
      "",
      NumberString(child_index + 1),
      "btn_outline_primary",
      select_page);
    icon_container_row.add(IconGridContainer(Names::icon_grid_container));

    auto icon_grid = icon_container_row.get_child(child_index)
                       .get<IconGridContainer>()
                       .get_icon_grid();

    add_icons(icon_grid, offset, bootstrap_icons_list, key_list);
  }
}

void IconMaker::select_page(lv_event_t *e) {
  const Event event(e);
  auto self = get_top_container(e);

  auto button = event.target().get<Button>();

  {
    auto button_row = button.get_parent();
    for (auto object : button_row) {
      if (object.name() != Names::select_all_button) {
        object.get<Button>().clear_state(State::checked);
      }
    }
    button.add_state(State::checked);
  }

  const auto page
    = StringView(button.get_child(0).get<Label>().get_text()).to_integer() - 1;
  const auto row = self.find(Names::icon_container_row);
  self.find<Row>(Names::icon_container_row)
    .scroll_to_x(row.get_width() * page, IsAnimate::no);
}

void IconMaker::select_all(lv_event_t *e) {
  const Event event(e);
  auto button = event.target<Button>();
  auto self = get_top_container(e);
  const auto row = self.find(Names::icon_container_row);
  for (auto container : row) {
    auto grid = container.get<IconGridContainer>().get_icon_grid();
    for (auto icon : grid) {
      icon.get<IconCheck>().set_checked(button.has_state(State::checked));
    }
  }
}

void IconMaker::deselect_all(lv_event_t *) {}

void IconMaker::add_icons(
  IconGrid icon_grid,
  u32 offset,
  json::JsonObject bootstrap_icons,
  const var::StringViewList &key_list) {

  for (const auto index : api::Index(IconGridContainer::max_icon_count)) {
    if (const auto index_offset = index + offset;
        index_offset < key_list.count()) {
      const auto key = key_list.at(index_offset);
      const u16 unicode = bootstrap_icons.at(key).to_integer();
      icon_grid.add(IconCheck(unicode, key, NumberString(unicode, "u%04x")));
    }
  }
}
