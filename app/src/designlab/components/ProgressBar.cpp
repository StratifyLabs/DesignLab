
#include <lvgl.hpp>
#include <design.hpp>

#include "ProgressBar.hpp"

ProgressBar::ProgressBar(const char * name){
  construct_object(name);
  add(bar(Names::progress_bar)
.set_width(100%)
  )
}
