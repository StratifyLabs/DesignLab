//
// Created by Tyler Gilbert on 3/3/22.
//

#ifndef DESIGNLAB_COMPONENT_TREE_HPP
#define DESIGNLAB_COMPONENT_TREE_HPP

#include <json/Json.hpp>
#include <lvgl/ObjectAccess.hpp>

#include "AddComponent.hpp"

class ComponentTree : public lvgl::ObjectAccess<ComponentTree> {
public:
  LVGL_OBJECT_ACCESS_DECLARE_CONSTRUCTOR(ComponentTree);

  ComponentTree& set_tree(const json::JsonObject tree);

  using Fields = AddComponent::Fields;

  static bool is_cancel_button(lv_event_t * e);

private:
  void add_leaf(lvgl::Generic generic, const json::JsonObject tree_leaf, int indentation);

};

#endif // DESIGNLAB_COMPONENT_TREE_HPP
