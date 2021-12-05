//
// Created by Tyler Gilbert on 12/2/21.
//

#ifndef DESIGNLAB_GUIOBJECT_HPP
#define DESIGNLAB_GUIOBJECT_HPP


#include <printer/YamlPrinter.hpp>
#include <thread/Mutex.hpp>
#include <thread/Thread.hpp>

using namespace printer;

#include <lvgl.hpp>

#include "fonts/Icons.hpp"

class GuiObject : public api::ExecutionContext {
public:
  static Printer &printer() { return model().printer; }

protected:

  struct Model {
  public:
    API_SINGLETON(Model);
    lvgl::Font title_font;
    Style column_flow_style;
    Style fill_parent_style;
    YamlPrinter printer;
    var::Queue<chrono::ClockTime::UniqueString> name_list;
    lv_obj_t * selected_object = nullptr;
    var::Queue<var::Vector<FormList::ItemData>> tree;
    lvgl::Runtime * runtime;
    lvgl::Theme theme;
  };

  static Model &model() { return Model::instance(); }

  static void go_back(lv_event_t*){

  }
};


#endif // DESIGNLAB_GUIOBJECT_HPP
