//
// Created by Tyler Gilbert on 12/16/21.
//

#include "ExportWorker.hpp"
#include "AssetManager.hpp"

#include "model/Model.hpp"


void ExportWorker::interface_work(){
  Model & model = Model::instance();

  printf("Doing export work\n");



}

/*
 * The constructor cannot use the model or it will
 * cause a recursive dependency with the model
 */
ExportWorker::ExportWorker(lvgl::Runtime *runtime) : WorkerObject(runtime){}
