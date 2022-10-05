//
// Created by Tyler Gilbert on 12/12/21.
//

#ifndef DESIGNLAB_FONTMAKER_HPP
#define DESIGNLAB_FONTMAKER_HPP

#include <design/extras/Form.hpp>

#include "ViewObject.hpp"
#include "extras/Extras.hpp"

class FontMaker : public ViewObject, public ObjectAccess<FontMaker> {
public:
  static void configure(Generic generic){
    generic.add(FontMaker(ViewObject::Names::font_maker_object));
  }

private:
  LVGL_OBJECT_ACCESS_DECLARE_CONSTRUCTOR(FontMaker);
};

#endif // DESIGNLAB_FONTMAKER_HPP
