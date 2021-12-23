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
  fs::PathList m_asset_path_list;
  fs::PathList m_theme_path_list;
  fs::PathList m_font_path_list;

  void interface_work();

  void export_assets(const Settings & settings);
  void export_themes(const Settings & settings);
  void export_fonts(const Settings & settings);
  void export_cmake_sourcelist(const Settings & settings);



};

#endif // DESIGNLAB_EXPORTWORKER_HPP
