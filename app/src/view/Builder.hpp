//
// Created by Tyler Gilbert on 2/24/22.
//

#ifndef DESIGNLAB_BUILDER_HPP
#define DESIGNLAB_BUILDER_HPP

#include "ViewObject.hpp"

#include "extras/BuilderTools.hpp"

class Builder : public ViewObject, public ObjectAccess<Builder> {
public:
  static void configure(Generic generic){
    generic.add(Builder(ViewObject::Names::builder_object));
  }

  LVGL_OBJECT_ACCESS_DECLARE_CONSTRUCTOR(Builder);

private:

  struct Data : public UserDataAccess<Data> {
    Data(const char * name) : UserDataAccess<Data>(name){}

    Style highlight_style;
    lv_obj_t * selected_object;
    json::JsonObject json_tree;
    var::String json_path;

  };

  struct Names {
    DESIGN_DECLARE_NAME(container);
    DESIGN_DECLARE_NAME(control_drawer);
    DESIGN_DECLARE_NAME_VALUE(target_object, ComponentBase);
    DESIGN_DECLARE_NAME(highlight_object);
    DESIGN_DECLARE_NAME(builder_tools);
    DESIGN_DECLARE_NAME(currently_selected_label);
    DESIGN_DECLARE_NAME(get_parent_button);
    DESIGN_DECLARE_NAME(get_previous_sibling_button);
    DESIGN_DECLARE_NAME(get_next_sibling_button);
  };


  BuilderTools get_builder_tools() const;

  static Builder get_builder(lv_event_t * e);
  static void show_clicked(lv_event_t*e);
  static void target_clicked(lv_event_t*e);
  static void builder_tools_clicked(lv_event_t*e);
  static void get_parent_clicked(lv_event_t*e);
  static void get_previous_sibling_clicked(lv_event_t*e);
  static void get_next_sibling_clicked(lv_event_t*e);

  Data * data(){
    return user_data<Data>();
  }


  Builder & add_component(json::JsonObject form_value);
  Builder & select_target(Object object);


};


#endif // DESIGNLAB_BUILDER_HPP
