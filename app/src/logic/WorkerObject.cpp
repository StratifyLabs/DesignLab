//
// Created by Tyler Gilbert on 12/20/21.
//

#include "WorkerObject.hpp"
#include "model/Model.hpp"

void WorkerObject::update_message(var::StringView message) {
  auto *notify_object = [&]() {
    Model::Scope model_scope;
    auto &model = ModelAccess::model();
    model.worker_message = message;
    return model.worker_notify_object;
  }();
  notify(notify_object);
}

void WorkerObject::update_progress(int value, int total) {
  auto *notify_object = [&]() {
    Model::Scope model_scope;
    auto &model = ModelAccess::model();
    model.worker_progress_range.set_value(value).set_maximum(total);
    return model.worker_notify_object;
  }();
  notify(notify_object);
}
