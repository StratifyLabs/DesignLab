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


  void interface_work();
};

#endif // DESIGNLAB_EXPORTWORKER_HPP
