#include <lvgl.hpp>

#include "FileBrowser.hpp"

void FileBrowser::configure(lvgl::Container &container) {
  static constexpr auto folder_tile_view_name = "fbFolderTileView";
  static constexpr auto home_tile_name = "fbHomeTile";


  static auto configure_folder = [](Container & container){
    container.add(Label("").configure([](Label&object){
      object.set_text_static("Folder");
    }));
  };


  // this needs forward/backward tileview navigation
  container.add(TileView(folder_tile_view_name).configure([](TileView &tile_view) {
    tile_view.set_width(100_percent)
      .set_height(100_percent)
      .add_tile(home_tile_name, TileView::Location(), configure_folder);
  }));


}


void FileBrowser::configure_directory(lvgl::Container & container){

  container.add(List("").configure([](List & list){
    list.set_width(90_percent).set_height(90_percent).add_event_callback(EventCode::notified, [](lv_event_t*e){
      const Event event(e);




    });
  }));
}
