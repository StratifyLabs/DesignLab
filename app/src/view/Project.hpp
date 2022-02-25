//
// Created by Tyler Gilbert on 12/15/21.
//

#ifndef DESIGNLAB_PROJECT_HPP
#define DESIGNLAB_PROJECT_HPP

#include "ViewObject.hpp"

class Project : public ViewObject, public ObjectAccess<Project> {
public:
  static void configure(Generic generic){
    generic.add(Project(ViewObject::Names::project_object));
  }

private:
  LVGL_OBJECT_ACCESS_DECLARE_CONSTRUCTOR(Project);
  struct Names {
    DESIGN_DECLARE_NAME(directory_select_file);
    DESIGN_DECLARE_NAME(source_select_file);
    DESIGN_DECLARE_NAME(lv_font_conv_select_file);
    DESIGN_DECLARE_NAME(node_select_file);
    DESIGN_DECLARE_NAME(project_form);
    DESIGN_DECLARE_NAME(project_column);
  };

  static void export_project(lv_event_t*);

  static void export_work(Worker * worker);

  static void configure_form(Form form);
  static void handle_exited(lv_event_t*);
  static void mark_all_as_dirty(lv_event_t*);
  static void project_path_changed(lv_event_t*);
  static void node_path_changed(lv_event_t*);
  static void lv_font_conv_path_changed(lv_event_t*);
  static void source_path_changed(lv_event_t*);

  static void accept_prompt_new_project(lv_event_t * e);


};

#endif // DESIGNLAB_PROJECT_HPP
