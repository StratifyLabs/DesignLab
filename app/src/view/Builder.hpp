//
// Created by Tyler Gilbert on 2/24/22.
//

#ifndef DESIGNLAB_BUILDER_HPP
#define DESIGNLAB_BUILDER_HPP

#include "ViewObject.hpp"

#include "extras/AddComponent.hpp"

class Builder : public ViewObject, public ObjectAccess<Builder> {
public:
  static void configure(Generic generic){
    generic.add(Builder(ViewObject::Names::builder_object));
  }

  LVGL_OBJECT_ACCESS_DECLARE_CONSTRUCTOR(Builder);

  using Fields = AddComponent::Fields;
  using Components = AddComponent::Components;

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

    DESIGN_DECLARE_NAME(add_component_modal);
    DESIGN_DECLARE_NAME(add_component);
    DESIGN_DECLARE_NAME(edit_component_modal);
    DESIGN_DECLARE_NAME(edit_component);

    DESIGN_DECLARE_NAME(remove_button);
    DESIGN_DECLARE_NAME(get_parent_button);
    DESIGN_DECLARE_NAME(get_previous_sibling_button);
    DESIGN_DECLARE_NAME(get_next_sibling_button);
  };


  static Builder get_builder(lv_event_t * e);
  static void remove_clicked(lv_event_t*e);
  static void edit_clicked(lv_event_t*e);
  static void add_clicked(lv_event_t*e);
  static void show_clicked(lv_event_t*e);
  static void target_clicked(lv_event_t*e);
  static void add_component_clicked(lv_event_t*e);
  static void edit_component_clicked(lv_event_t*e);
  static void get_parent_clicked(lv_event_t*e);
  static void get_previous_sibling_clicked(lv_event_t*e);
  static void get_next_sibling_clicked(lv_event_t*e);

  Data * data(){
    return user_data<Data>();
  }

  const Data * data() const {
    return user_data<const Data>();
  }


  Builder & add_component(json::JsonObject form_value);
  Builder & edit_component(json::JsonObject form_value);
  Builder & select_target(Object object);
  Builder & remove_selected();

  json::JsonObject get_active_json_object() const;

};


#endif // DESIGNLAB_BUILDER_HPP
