
#include <design.hpp>
#include <lvgl.hpp>

#include "MyComponent2.hpp"

MyComponent2::MyComponent2(const char *name) {
  construct_object(name);
  add(Column(Names::main_column)
        .set_height(100 %)
        .set_width(100 %)
        .add(Heading1(Names::main_heading))
        .add(Bar(Names::progress_bar).set_width(100 %))
        .add(Row(Names::button_row)
               .set_width(100 %)
               .add(Button(Names::up_button).add_label_as_static("Up"))
               .add(Button(Names::down_button).add_label_as_static("Down"))))
}
