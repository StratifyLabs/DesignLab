//
// Created by Tyler Gilbert on 12/8/21.
//

#ifndef DESIGNLAB_HOME_HPP
#define DESIGNLAB_HOME_HPP

#include "ViewObject.hpp"

class Home : public ViewObject, public ObjectAccess<Home> {
public:
  static void configure(Generic generic){
    generic.add(Home(ViewObject::Names::home_object));
  }

private:
  LVGL_OBJECT_ACCESS_DECLARE_CONSTRUCTOR(Home);

};

#endif // DESIGNLAB_HOME_HPP
