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

  };

  struct Names {
    DESIGN_DECLARE_NAME(container);
    DESIGN_DECLARE_NAME(control_drawer);
    DESIGN_DECLARE_NAME(target_object);
    DESIGN_DECLARE_NAME(builder_tools);
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

  Builder& add_label(const char * name);
  Builder& add_row();

  Data * data(){
    return user_data<Data>();
  }

  void update_target_properties(Generic generic);


};


#endif // DESIGNLAB_BUILDER_HPP
