//
// Created by Tyler Gilbert on 12/12/21.
//

#ifndef DESIGNLAB_ICONMAKER_HPP
#define DESIGNLAB_ICONMAKER_HPP

#include "extras/Extras.hpp"
#include "Maker.hpp"

class IconMaker : public Maker {
public:
  static void configure(Generic generic);

private:

  struct Names {
    static constexpr auto control_row = "ControlRow";
    static constexpr auto icon_grid = "IconGrid";
  };

  static void configure_control_row(Row row);
  static void add_icons(IconGrid icon_grid);

  static void select_all(lv_event_t*);
  static void deselect_all(lv_event_t*);

};

#endif // DESIGNLAB_ICONMAKER_HPP
