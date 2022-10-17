#include <lvgl.hpp>
#include <design.hpp>

#include "MyComponent2.hpp"

MyComponent2::MyComponent2(const char * name){
  construct_object(name);
  add(Column(Names::main_column)
.set_height(100_percent)
.set_width(100_percent)
  .add(Heading1(Names::main_heading)
.set_x({object})
.set_text("")
  )
  .add(Bar(Names::progress_bar)
.set_width(100_percent)
  )
  .add(Row(Names::button_row)
.set_width(100_percent)
  .add(Button(Names::up_button)
  )
  .add(Button(Names::down_button)
.set_radius(20)
  )
  .add(Slider(Names::MySlider)
.set_height(40)
.set_width(75_percent)
  )
  )
  );
}
