
#include <lvgl.hpp>
#include <design.hpp>

#include "SignupForm.hpp"

SignupForm::SignupForm(const char * name){
  construct_object(name);
  add(Form(Names::form_container)
.set_height(100%)
.set_width(100%)
  .add(Form::SectionHeading(Names::Details)
  )
  .add(Form::LineField(Names::email_line_field)
.set_label_as_static("")
.set_hint_as_static("")
  )
  .add(Form::LineField(Names::password_line_field)
.set_label_as_static("")
.set_hint_as_static("")
  )
  )
}
