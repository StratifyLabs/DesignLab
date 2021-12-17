//
// Created by Tyler Gilbert on 12/16/21.
//

#ifndef DESIGNLAB_EXPORTWORKER_HPP
#define DESIGNLAB_EXPORTWORKER_HPP

#include "WorkerObject.hpp"

class ExportWorker : public WorkerObject {
public:
  ExportWorker() = default;
  ExportWorker(lvgl::Runtime * runtime);
protected:

  var::PathString m_project_path;

  void interface_work();

  void export_assets(const Settings & settings);
  void export_themes(const Settings & settings);
};

#endif // DESIGNLAB_EXPORTWORKER_HPP
