//
// Created by Tyler Gilbert on 3/1/22.
//

#ifndef DESIGNLAB_EDITCOMPONENT_HPP
#define DESIGNLAB_EDITCOMPONENT_HPP

#include <design/extras/Form.hpp>
#include <design/macros.hpp>
#include <lvgl/ObjectAccess.hpp>

class EditComponent : public lvgl::ObjectAccess<EditComponent> {
public:
  LVGL_OBJECT_ACCESS_DECLARE_CONSTRUCTOR(EditComponent);

private:
  struct Names {
    DESIGN_DECLARE_NAME(top_container);
    DESIGN_DECLARE_NAME(parent_label);
    DESIGN_DECLARE_NAME(get_parent_button);
    DESIGN_DECLARE_NAME(get_previous_sibling_button);
    DESIGN_DECLARE_NAME(get_next_sibling_button);
    DESIGN_DECLARE_NAME(add_button);
    DESIGN_DECLARE_NAME(object_dropdown);
    DESIGN_DECLARE_NAME(column);
    DESIGN_DECLARE_NAME(form);
    DESIGN_DECLARE_NAME(fill_width_button);
    DESIGN_DECLARE_NAME(fill_height_button);

    DESIGN_DECLARE_NAME(main_row);
    DESIGN_DECLARE_NAME(control_column);
    DESIGN_DECLARE_NAME(form_column);

    DESIGN_DECLARE_NAME(lvgl_row);
    DESIGN_DECLARE_NAME(design_row);
    DESIGN_DECLARE_NAME(extra_design_row);
  };
};

#endif // DESIGNLAB_EDITCOMPONENT_HPP
