//
// Created by Tyler Gilbert on 12/16/21.
//

#ifndef DESIGNLAB_MODEL_HPP
#define DESIGNLAB_MODEL_HPP

#include <lvgl/Runtime.hpp>
#include <lvgl/Theme.hpp>
#include <printer/YamlPrinter.hpp>

#include <design/extras/Form.hpp>

#include "Settings.hpp"
#include "logic/ExportWorker.hpp"

struct Model {
public:
  printer::YamlPrinter printer;
  lvgl::Runtime *runtime;
  ExportWorker export_worker;

  SessionSettings session_settings;
  Settings project_settings;

  lvgl::Theme theme;
  design::Form::Schema font_input_form_schema;
  design::Form::Schema asset_input_form_schema;
  design::Form::Schema theme_input_form_schema;
  design::Form::Schema icon_input_form_schema;

  class Scope : public thread::Mutex::Scope {
  public:
    Scope() : thread::Mutex::Scope(Model::instance().m_mutex) {
      auto &model = Model::instance();
      model.m_pthread_scoped = thread::Thread::self();
      ++model.m_lock_count;
    }

    ~Scope() {
      auto &model = Model::instance();
      --model.m_lock_count;
      if (model.m_lock_count == 0) {
        model.m_pthread_scoped = {};
      }
    }
  };

private:
  friend Scope;
  friend class ModelAccess;

  thread::Mutex m_mutex;
  pthread_t m_pthread_scoped = pthread_t{};
  size_t m_lock_count = 0;
  API_SINGLETON_CUSTOM_CONSTRUCTOR(Model);

  Model()
    : project_settings(
      Settings::get_file_path(session_settings.get_project()),
      Settings::IsOverwrite::yes),
      m_mutex(
        thread::Mutex::Attributes().set_type(thread::Mutex::Type::recursive)) {}
};

class ModelAccess : public api::ExecutionContext {
public:
  static Model &model() {
    // make sure the caller has locked the model
    Model &result = Model::instance();
    API_ASSERT(result.m_pthread_scoped == thread::Thread::self());
    return Model::instance();
  }
  static printer::Printer &printer() { return model().printer; }
};

#endif // DESIGNLAB_MODEL_HPP
