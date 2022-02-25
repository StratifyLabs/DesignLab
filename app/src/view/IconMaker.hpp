//
// Created by Tyler Gilbert on 12/12/21.
//

#ifndef DESIGNLAB_ICONMAKER_HPP
#define DESIGNLAB_ICONMAKER_HPP

#include "ViewObject.hpp"
#include "extras/Extras.hpp"

class IconMaker : public ViewObject, public ObjectAccess<IconMaker> {
public:
  static void configure(Generic generic){
    generic.add(IconMaker(ViewObject::Names::icon_maker_object));
  }

private:
  LVGL_OBJECT_ACCESS_DECLARE_CONSTRUCTOR(IconMaker);

  using RangeList = var::Vector<Settings::Icon>;

  struct Names {
    static constexpr auto control_row = "ControlRow";
    static constexpr auto select_all_button = "SelectAllButton";
    static constexpr auto icon_container_row = "IconContainerRow";
    static constexpr auto icon_grid = "IconGrid";
    static constexpr auto icon_grid_container = "IconGridContainer";
    static constexpr auto input_form_column = "InputFormColumn";
  };

  static u32 add_icons(
    IconGrid icon_grid,
    u32 offset,
    const var::Vector<Settings::Icon> &icon_list);

  static void select_all(lv_event_t *);
  static void select_page(lv_event_t *e);

  static Container get_top_container(lv_event_t *e) {
    return Event(e).target().find_parent<Container>(
      ViewObject::Names::content_container);
  }

  template <class Context>
  static void update_icons(
    Container self,
    Context &context,
    void (*callback)(IconCheck, Context &)) {
    const auto row = self.find(Names::icon_container_row);
    for (auto container : row) {
      auto grid = container.get<IconGridContainer>().get_icon_grid();
      for (auto icon : grid) {
        callback(icon.get<IconCheck>(), context);
      }
    }
  }

  static void handle_exited(lv_event_t *);

  static var::Vector<InfoCard::Data::Feature>
    get_feature_list(json::JsonObject);
  static var::StringView get_info_title(json::JsonObject object);
};

#endif // DESIGNLAB_ICONMAKER_HPP
