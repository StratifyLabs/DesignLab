//
// Created by Tyler Gilbert on 12/12/21.
//

#ifndef DESIGNLAB_ICONMAKER_HPP
#define DESIGNLAB_ICONMAKER_HPP

#include "ViewObject.hpp"
#include "extras/Extras.hpp"

class IconMaker : public ViewObject {
public:
  static void configure(Generic generic);

private:
  struct Names {
    static constexpr auto first_child_container = "FirstChildContainer";
    static constexpr auto control_row = "ControlRow";
    static constexpr auto select_all_button = "SelectAllButton";
    static constexpr auto icon_container_row = "IconContainerRow";
    static constexpr auto icon_grid = "IconGrid";
    static constexpr auto icon_grid_container = "IconGridContainer";
    static constexpr auto input_form_column = "InputFormColumn";
  };

  static void add_icons(
    IconGrid icon_grid,
    u32 offset,
    json::JsonObject bootstrap_icons,
    const var::StringViewList &key_list);

  static void select_all(lv_event_t *);
  static void deselect_all(lv_event_t *);
  static void select_page(lv_event_t *e);

  static Container get_top_container(lv_event_t *e) {
    return Event(e).target().find_parent<Container>(
      Names::first_child_container);
  }
};

#endif // DESIGNLAB_ICONMAKER_HPP
