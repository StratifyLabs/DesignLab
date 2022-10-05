//
// Created by Tyler Gilbert on 12/12/21.
//

#ifndef DESIGNLAB_ICONMAKER_HPP
#define DESIGNLAB_ICONMAKER_HPP

#include "ViewObject.hpp"
#include "extras/Extras.hpp"

class IconMaker : public ViewObject, public ObjectAccess<IconMaker> {
public:
  static void configure(Generic generic){
    generic.add(IconMaker(ViewObject::Names::icon_maker_object));
  }

private:
  LVGL_OBJECT_ACCESS_DECLARE_CONSTRUCTOR(IconMaker);
};

#endif // DESIGNLAB_ICONMAKER_HPP
