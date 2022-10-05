//
// Created by Tyler Gilbert on 12/12/21.
//

#ifndef DESIGN_LAB_COMPONENT_MAKER_HPP
#define DESIGN_LAB_COMPONENT_MAKER_HPP

#include <design/extras/Form.hpp>

#include "ViewObject.hpp"
#include "extras/Extras.hpp"

class ComponentMaker : public ViewObject, public ObjectAccess<ComponentMaker> {
public:
  static void configure(Generic generic){
    generic.add(ComponentMaker(ViewObject::Names::component_maker_object));
  }
  LVGL_OBJECT_ACCESS_DECLARE_CONSTRUCTOR(ComponentMaker);

private:


};

#endif // DESIGN_LAB_COMPONENT_MAKER_HPP
