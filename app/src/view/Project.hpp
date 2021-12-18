//
// Created by Tyler Gilbert on 12/15/21.
//

#ifndef DESIGNLAB_PROJECT_HPP
#define DESIGNLAB_PROJECT_HPP

#include "ViewObject.hpp"

class Project : public ViewObject {
public:
  static void configure(Generic generic);
private:

  struct Names {
    static constexpr auto directory_select_file = "DirectorySelectFile";
    static constexpr auto source_select_file = "SourceSelectFile";
    static constexpr auto project_form = "ProjectForm";
    static constexpr auto project_column = "ProjectColumn";
  };

  static void export_project(lv_event_t*);

  static void export_work(Worker * worker);

  static void configure_form(Form form);
  static void handle_exited(lv_event_t*);


};

#endif // DESIGNLAB_PROJECT_HPP
