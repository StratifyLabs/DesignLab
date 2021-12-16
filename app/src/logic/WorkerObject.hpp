//
// Created by Tyler Gilbert on 12/16/21.
//

#ifndef DESIGNLAB_WORKEROBJECT_HPP
#define DESIGNLAB_WORKEROBJECT_HPP

#include <api/api.hpp>
#include <design/Worker.hpp>

class WorkerObject : public design::Worker {
public:
  WorkerObject() = default;
  WorkerObject(lvgl::Runtime * runtime) : design::Worker(runtime, work){}

  static void work(Worker * worker){
    static_cast<WorkerObject*>(worker)->interface_work();
  }

protected:



  virtual void interface_work() = 0;

};

#endif // DESIGNLAB_WORKEROBJECT_HPP