//
// Created by Tyler Gilbert on 12/2/21.
//

#ifndef DESIGNLAB_SCREENOBJECT_HPP
#define DESIGNLAB_SCREENOBJECT_HPP

#include "printer/YamlPrinter.hpp"
#include "thread/Mutex.hpp"
#include "thread/Thread.hpp"

using namespace printer;

#include <lvgl.hpp>
#include <design.hpp>

#include <design/extras/FormList.hpp>

#include "fonts/Icons.hpp"

class ScreenObject : public api::ExecutionContext {
public:
  static Printer &printer() { return model().printer; }

protected:

  using ScreenHeading = Heading1;
  using SectionHeading = Heading2;
  using SubSectionHeading = Heading3;

  struct Model {
  public:
    API_SINGLETON(Model);
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


#endif // DESIGNLAB_SCREENOBJECT_HPP
