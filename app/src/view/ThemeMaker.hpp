//
// Created by Tyler Gilbert on 12/8/21.
//

#ifndef DESIGNLAB_THEMEMAKER_HPP
#define DESIGNLAB_THEMEMAKER_HPP

#include "ViewObject.hpp"
#include "extras/Extras.hpp"

#include "Editor.hpp"

class ThemeMaker : public ViewObject, public ObjectAccess<ThemeMaker> {
public:
  static void configure(lvgl::Generic generic){
    generic.add(ThemeMaker(ViewObject::Names::theme_maker_object));
  }

private:
  LVGL_OBJECT_ACCESS_DECLARE_CONSTRUCTOR(ThemeMaker);

};

#endif // DESIGNLAB_THEMEMAKER_HPP
