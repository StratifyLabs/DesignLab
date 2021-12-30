//
// Created by Tyler Gilbert on 12/16/21.
//

#ifndef DESIGNLAB_EXPORTWORKER_HPP
#define DESIGNLAB_EXPORTWORKER_HPP

#include <fs/FileSystem.hpp>

#include "WorkerObject.hpp"

class Settings;
class ExportWorker : public WorkerObject {
public:
  ExportWorker() = default;
  ExportWorker(lvgl::Runtime * runtime);
protected:

  var::PathString m_project_path;
  Settings m_project_settings;
  fs::PathList m_asset_path_list;
  fs::PathList m_theme_path_list;
  fs::PathList m_font_path_list;

  void interface_work();

  void export_assets();
  void export_themes();
  fs::PathList get_font_path_list();
  void export_fonts();
  void export_cmake_sourcelist();

  static void update_font_progress_callback(void * context, int value, int total){
    reinterpret_cast<ExportWorker*>(context)->update_font_progress(value, total);
  }

  void update_font_progress(int value, int total);



};

#endif // DESIGNLAB_EXPORTWORKER_HPP
