//
// Created by Tyler Gilbert on 2/24/22.
//

#ifndef DESIGNLAB_BUILDER_HPP
#define DESIGNLAB_BUILDER_HPP

#include "ViewObject.hpp"

#include "extras/AddComponent.hpp"

class Builder : public ViewObject, public ObjectAccess<Builder> {
public:
  static void configure(Generic generic) {
    generic.add(Builder(ViewObject::Names::builder_object));
  }

  LVGL_OBJECT_ACCESS_DECLARE_CONSTRUCTOR(Builder);

  using Fields = AddComponent::Fields;
  using Components = AddComponent::Components;

  static bool is_back_button(lv_event_t *e);

  Builder &set_component_name(var::StringView name) {
    data()->component_name = name;
    return *this;
  }

  const var::GeneralString &component_name() const {
    return data()->component_name;
  }

  Builder &load_json_tree(Settings::Component component);

  Builder &set_component_offset(u32 value) {
    data()->component_offset = value;
    return *this;
  }

  u32 component_offset() const { return data()->component_offset; }

  json::JsonObject get_json_tree() const { return data()->json_tree; }

  bool is_new() const { return data()->is_new; }

  struct Data : public UserDataAccess<Data> {
    Data(const char *name) : UserDataAccess<Data>(name) {}
    Style highlight_style;
    lv_obj_t *selected_object;
    json::JsonObject json_tree;
    var::String json_path;
    var::GeneralString component_name;
    u32 component_offset;
    bool is_new = true;
  };

  Data *data() { return user_data<Data>(); }

  const Data *data() const { return user_data<const Data>(); }

  Builder &add_component(const json::JsonObject &form_value);
  Builder &edit_component(const json::JsonObject &form_value);
  Builder &select_target(const Object &object);
  Builder &remove_selected();

  json::JsonObject get_active_json_object() const;

  void build_tree(lv_obj_t *target, const json::JsonObject &object);
};

#endif // DESIGNLAB_BUILDER_HPP
