
#ifndef DESIGN_LAB_COMPONENTS_SIGNUPFORM_HPP
#define DESIGN_LAB_COMPONENTS_SIGNUPFORM_HPP


#include <lvgl/ObjectAccess.hpp>
#include <design/macros.hpp>


class SignupForm : public lvgl::ObjectAccess<SignupForm> {
public:
  LVGL_OBJECT_ACCESS_DECLARE_CONSTRUCTOR(SignupForm);

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
    DESIGN_DECLARE_NAME(form_container);
    DESIGN_DECLARE_NAME(Details);
    DESIGN_DECLARE_NAME(email_line_field);
    DESIGN_DECLARE_NAME(password_line_field);
  };
};
#endif //DESIGN_LAB_COMPONENTS_SIGNUPFORM_HPP;

