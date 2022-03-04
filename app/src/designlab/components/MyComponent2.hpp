
#ifndef DESIGN_LAB_COMPONENTS_MYCOMPONENT2_HPP
#define DESIGN_LAB_COMPONENTS_MYCOMPONENT2_HPP


#include <lvgl/ObjectAccess.hpp>
#include <design/macros.hpp>


class MyComponent2 : public lvgl::ObjectAccess<MyComponent2> {
public:
  LVGL_OBJECT_ACCESS_DECLARE_CONSTRUCTOR(MyComponent2);

  struct Data : lvgl::UserDataAccess<Data> {
    Data(const char * name = "") : lvgl::UserDataAccess<Data>(name){}
  };

  class Worker : design::WorkerAccess<Worker> {
public:
    Worker() = default;
    Worker(lvgl::Runtime * runtime) : design::WorkerAccess(runtime){}

private:
    void work() override;
  };

private:
  struct Names {
    DESIGN_DECLARE_NAME(main_column);
    DESIGN_DECLARE_NAME(main_heading);
    DESIGN_DECLARE_NAME(progress_bar);
    DESIGN_DECLARE_NAME(button_row);
    DESIGN_DECLARE_NAME(up_button);
    DESIGN_DECLARE_NAME(down_button);
  };
};
#endif //DESIGN_LAB_COMPONENTS_MYCOMPONENT2_HPP;

