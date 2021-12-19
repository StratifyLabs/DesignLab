//
// Created by Tyler Gilbert on 12/12/21.
//

#include <lvgl.hpp>
#include <var.hpp>

#include "IconMaker.hpp"
#include "designlab/icon_fonts.h"
#include "extras/Extras.hpp"

void IconMaker::configure(Generic generic) {

  generic.clear_flag(Flags::scrollable)
    .add(Container(ViewObject::Names::content_container)
           .add_event_callback(EventCode::exited, handle_exited)
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

  const auto icons_list = [&]() {
    var::Vector<Settings::Icon> result;
    auto load_file = [&](var::StringView family, var::StringView path) {
      auto icons_object
        = json::JsonDocument().load(AssetFile(path)).to_object();
      const auto key_list = icons_object.get_key_list();
      for (const auto &key : key_list) {
        result.push_back(
          Settings::Icon().set_family(family).set_name(key).set_unicode(
            icons_object.at(key).to_string_view()));
      }
    };
    load_file("solid", "a:fa-solid.json");
    load_file("regular", "a:fa-regular.json");
    load_file("brands", "a:fa-brands.json");
    return result;
  }();

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
    icons::fa::plus_solid,
    " Select All",
    "",
    select_all);

  // push the page buttons to the right
  control_row.add(NakedContainer().set_height(1).set_flex_grow());

  auto icon_container_row = generic.find<Row>(Names::icon_container_row);

  const auto icon_count = icons_list.count();
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

    add_icons(icon_grid, offset, icons_list);
  }

  auto range = []() {
    Model::Scope model_scope;
    StringList result;
    return model().project_settings.icons().get_range();
  }();

  update_icons<RangeList>(
    generic.find<Container>(ViewObject::Names::content_container),
    range,
    [](IconCheck icon_check, RangeList &range) {
      const StringView unicode = icon_check.get_unicode();
      bool is_checked = false;
      for (const auto &in_range : range) {
        if (
          in_range.get_unicode() == unicode
          && in_range.get_family() == icon_check.get_family()) {
          is_checked = true;
          break;
        }
      }
      icon_check.set_checked(is_checked);
    });
}

u32 IconMaker::add_icons(
  IconGrid icon_grid,
  u32 offset,
  const var::Vector<Settings::Icon> &icon_list) {
  u32 count = 0;

  auto get_font = [](var::StringView family) {
    if (family == "solid") {
      return Font(&fontawesome_solid);
    }

    if (family == "regular") {
      return Font(&fontawesome_regular);
    }

    return Font(&fontawesome_brands_regular);
  };

  for (const auto index : api::Index(IconGridContainer::max_icon_count)) {
    if (const auto index_offset = index + offset;
        index_offset < icon_list.count()) {
      const auto icon = icon_list.at(index_offset);
      const u16 unicode
        = icon.get_unicode().to_unsigned_long(StringView::Base::hexadecimal);

      const auto family = icon.get_family();

      icon_grid.add(
        IconCheck(unicode, icon.get_name(), family, get_font(family)));
      ++count;
    }
  }
  return count;
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
  bool value = button.has_state(State::checked);
  update_icons<bool>(self, value, [](IconCheck icon_check, bool &context) {
    icon_check.set_checked(context);
  });
}

void IconMaker::handle_exited(lv_event_t *e) {
  Model::Scope model_scope;
  // grab the selected icons and save them to the project settings

  const Event event(e);
  auto self = event.target<Container>();
  using IconQueue = Queue<Settings::Icon>;
  IconQueue icon_queue;

  update_icons<IconQueue>(
    self,
    icon_queue,
    [](IconCheck icon_check, IconQueue &icon_queue) {
      if (icon_check.is_checked()) {
        icon_queue.push(Settings::Icon()
                          .set_family(icon_check.get_family())
                          .set_name(icon_check.get_name())
                          .set_unicode(icon_check.get_unicode()));
      }
    });

  {
    Model::Scope scope;
    RangeList range_list;
    range_list.reserve(icon_queue.count());
    for (const auto icon : icon_queue) {
      range_list.push_back(icon);
    }
    model().project_settings.set_icons(
      Settings::Icons().set_name("icons").set_path("").set_range(range_list));

    printer().object("settings", model().project_settings);
  }
}

var::Vector<InfoCard::Data::Feature>
IconMaker::get_feature_list(json::JsonObject object) {
  var::Vector<InfoCard::Data::Feature> result;
  Settings::Icons item(object);

  result
    .push_back(
      {.icon = icons::fa::check_solid,
       .label = "name",
       .value = item.get_name()})
    .push_back(
      {.icon = icons::fa::file_solid,
       .label = "count",
       .value = NumberString(5).cstring()});
  return result;
}

var::StringView IconMaker::get_info_title(json::JsonObject object) {
  Settings::Icons item(object);
  return fs::Path::name(item.get_name());
}
