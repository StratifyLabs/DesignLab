#ifndef TREE_HPP
#define TREE_HPP

#include <lvgl/List.hpp>
#include <lvgl/TileView.hpp>

#include "Elements.hpp"

class Tree : public Application {
public:
  static void configure(Container &container) {

    static constexpr auto tile_view_name = "TileView";

    container.add(E::Title("", "Tree"))
      .add(E::Spacer())
      .add(E::PropertySectionTitle("Object"))
      .add(TileView(tile_view_name).configure([](TileView &tile_view) {
        tile_view.set_width(100_percent)
          .set_height(80_percent)
          .add_tile("", TileView::Location(), [](Container &container) {
            container.add(List("").configure([](List &list) {
              list.add_style(model().fill_parent_style);
              auto * canvas = Container::active_screen().find(canvas_container_name).object();
              configure_list(list, canvas);
            }));
          });
      })).add_event_callback(EventCode::notified, [](lv_event_t*){
        //refresh all tile view items


      });
  }

private:
  static void configure_list(List &list, lv_obj_t * object) {
    class Branch : public UserDataAccess<Branch> {
    public:
      Branch(const char * name) : UserDataBase(name){}

    private:
      API_AC(Branch, var::KeyString, class_name);
    };


    auto container = Container(object);
    for(u32 i=0; i < container.get_child_count(); i++){
      list.add_button(Branch::create(""), "", container.get_child(i).name(), [](Button & button){
        auto leaf = button.user_data<Branch>();



      });
    }



  }
};

#endif // TREE_HPP
