//
// Created by Tyler Gilbert on 3/3/22.
//

#include <design.hpp>
#include <lvgl.hpp>

#include "ComponentTree.hpp"

ComponentTree::ComponentTree(const char *name) {
  construct_object(name);
  set_width(90_percent);
  set_height(90_percent);
  add_style("card");
  add_style("container");

  add(Column(Names::main_column)
        .fill()
        .add_flag(Flags::event_bubble)
        .add(Row()
               .add_flag(Flags::event_bubble)
               .fill_width()
               .add(Heading1("Component Tree").set_flex_grow())
               .add(Button(Names::cancel_button)
                      .add_label_as_static(icons::fa::times_solid)
                      .add_style("btn_danger")
                      .add_flag(Flags::event_bubble)))
        .add(HorizontalLine())
        .add(Column(Names::tree_container)
               .fill_width()
               .add_flag(Flags::event_bubble)));
}

ComponentTree &ComponentTree::set_tree(const json::JsonObject tree) {
  auto tree_container = find<Generic>(Names::tree_container);
  add_leaf(tree_container, tree, 0);
  return *this;
}

void ComponentTree::add_leaf(
  lvgl::Generic generic,
  const json::JsonObject tree_leaf,
  int indentation) {

  const auto key_container = tree_leaf.get_key_list();
  for (const auto &key : key_container) {
    auto leaf = tree_leaf.at(key);
    if (leaf.is_object()) {
      const auto name
        = leaf.to_object().at(Fields::component_name).to_cstring();

      printf("add leaf %s %d\n", name, indentation);
      generic.add(NakedContainer()
                    .fill_width()
                    .add_flag(Flags::event_bubble)
                    .set_height(size_from_content)
                    .add(Button(name)
                           .add_label_as_static(name)
                           .add_style("btn_outline_primary")
                           .add_flag(Flags::event_bubble)
                           .set_x(indentation)));

      add_leaf(generic, leaf, indentation + 20);
    }
  }
}
