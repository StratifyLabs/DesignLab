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

  struct Names {
    DESIGN_DECLARE_NAME(main_column);
    DESIGN_DECLARE_NAME(introduction_paragraph);
    DESIGN_DECLARE_NAME(project_paragraph);
    DESIGN_DECLARE_NAME(theme_paragraph);
    DESIGN_DECLARE_NAME(asset_paragraph);
    DESIGN_DECLARE_NAME(font_paragraph);
    DESIGN_DECLARE_NAME(icon_paragraph);
    DESIGN_DECLARE_NAME(component_paragraph);
    DESIGN_DECLARE_NAME(color_paragraph);
  };

  static var::String remove_newlines(const char * paragraph);

};

#endif // DESIGNLAB_GETTING_STARTED_HPP
