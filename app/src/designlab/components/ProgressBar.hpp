
#ifndef DESIGN_LAB_COMPONENTS_PROGRESSBAR_HPP
#define DESIGN_LAB_COMPONENTS_PROGRESSBAR_HPP


#include <lvgl/ObjectAccess.hpp>
#include <design/macros.hpp>


class ProgressBar : public lvgl::ObjectAccess<ProgressBar> {
public:
  LVGL_OBJECT_ACCESS_DECLARE_CONSTRUCTOR(ProgressBar);



private:
  struct Names {
    DESIGN_DECLARE_NAME(progress_bar);
  };
};
#endif //DESIGN_LAB_COMPONENTS_PROGRESSBAR_HPP;

