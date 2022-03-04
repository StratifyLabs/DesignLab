//
// Created by Tyler Gilbert on 12/16/21.
//

#ifndef DESIGNLAB_MODEL_HPP
#define DESIGNLAB_MODEL_HPP

#include <lvgl/Runtime.hpp>
#include <lvgl/Theme.hpp>
#include <printer/YamlPrinter.hpp>

#include <design/extras/Form.hpp>
#include <design/Worker.hpp>

#include "Settings.hpp"

struct Model {
public:
  static constexpr auto worker_done_message = "Done";

  // all access to the model must be withing a Scope
  class Scope : public design::ModelScope {
  public:
    Scope() : design::ModelScope(Model::instance().model_scope_options) {}
  };

  printer::YamlPrinter printer;
  lvgl::Runtime *runtime = nullptr;
  var::GeneralString worker_message;
  lvgl::Range worker_progress_range;
  lv_obj_t * worker_notify_object;
  bool is_project_path_valid = false;
  bool is_theme_updated = false;
  bool is_dark_theme = false;
  var::PathString new_project_path;

  SessionSettings session_settings;
  Settings project_settings;

  lvgl::Theme light_theme;
  lvgl::Theme dark_theme;

private:
  friend Scope;
  friend class ModelAccess;

  design::ModelScope::Construct model_scope_options;

  Model()
    : project_settings(
      Settings::get_file_path(session_settings.get_project()),
      Settings::IsOverwrite::yes) {}

  static Model &instance() {
    static Model model;
    return model;
  }
};

class ModelAccess : public api::ExecutionContext {
public:
  static Model &model() {
    // make sure the caller has locked the model
    Model &result = Model::instance();
    API_ASSERT(result.model_scope_options.is_available());
    return Model::instance();
  }
  static printer::Printer &printer() { return model().printer; }

  static var::PathString get_project_path() {
    Model::Scope model_scope;
    return model().session_settings.get_project();
  }
};

#endif // DESIGNLAB_MODEL_HPP
