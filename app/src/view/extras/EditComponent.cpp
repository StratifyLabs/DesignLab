//
// Created by Tyler Gilbert on 3/1/22.
//

#include <design.hpp>
#include <lvgl.hpp>
#include <var.hpp>

#include "EditComponent.hpp"

EditComponent::EditComponent(const char *name) {
  construct_object(name);
  set_width(90_percent);
  set_height(90_percent);
  add_style("card");

  add(Column(Names::top_container).fill().add_style("container"));

  auto column = find<Column>(Names::top_container);

  column.add_flag(Flags::event_bubble)
    .add(
      Row(Names::main_row)
        .add_flag(Flags::event_bubble)
        .fill()
        .add(Column(Names::control_column)
               .add_event_callback(EventCode::clicked, nullptr)
               .fill_height()
               .set_width(30_percent))
        .add(Column(Names::form_column)
               .add_flag(Flags::event_bubble)
               .set_flex_grow()
               .add(Form(Names::form)
                      .fill_width()
                      .set_height(Form::size_from_content))));
}
