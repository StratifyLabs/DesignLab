//
// Created by Tyler Gilbert on 12/29/21.
//

#ifndef DESIGNLAB_GETTING_STARTED_HPP
#define DESIGNLAB_GETTING_STARTED_HPP

#include "ViewObject.hpp"

class GettingStarted : public ViewObject, public ObjectAccess<GettingStarted> {
public:
  static void configure(Generic generic){
    generic.add(GettingStarted(ViewObject::Names::about_object));
  }

private:
  LVGL_OBJECT_ACCESS_DECLARE_CONSTRUCTOR(GettingStarted);
};

#endif // DESIGNLAB_GETTING_STARTED_HPP
