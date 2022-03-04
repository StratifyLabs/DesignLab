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

  static bool is_cancel_button(lv_event_t * e){
    return Event(e).target().name() == Names::cancel_button;
  }

private:
  struct Names {
    DESIGN_DECLARE_NAME(main_column);
    DESIGN_DECLARE_NAME(tree_container);
    DESIGN_DECLARE_NAME(cancel_button);
  };

  void add_leaf(lvgl::Generic generic, const json::JsonObject tree_leaf, int indentation);

  static void cancel_clicked(lv_event_t*e);

};

#endif // DESIGNLAB_COMPONENT_TREE_HPP
