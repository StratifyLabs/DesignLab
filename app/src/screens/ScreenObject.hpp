//
// Created by Tyler Gilbert on 12/2/21.
//

#ifndef DESIGNLAB_SCREENOBJECT_HPP
#define DESIGNLAB_SCREENOBJECT_HPP

#include "printer/YamlPrinter.hpp"
#include "thread/Mutex.hpp"
#include "thread/Thread.hpp"

using namespace printer;

#include <design.hpp>
#include <lvgl.hpp>

#include <design/extras/FormList.hpp>
#include <design/extras/Form.hpp>

#include "fonts/Icons.hpp"
#include "logic/Manager.hpp"

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
    lv_obj_t *selected_object = nullptr;
    var::Queue<var::Vector<FormList::ItemData>> tree;
    lvgl::Runtime *runtime;
    lvgl::Theme theme;
    Manager::Settings project_settings;
    Form::Schema font_input_form_schema;
    Form::Schema asset_input_form_schema;
    Form::Schema theme_input_form_schema;
    Form::Schema icon_input_form_schema;
  };


  static Model &model() { return Model::instance(); }

  static void go_back(lv_event_t *) {}


};

#endif // DESIGNLAB_SCREENOBJECT_HPP
