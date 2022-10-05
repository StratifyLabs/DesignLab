//
// Created by Tyler Gilbert on 12/16/21.
//

#ifndef DESIGNLAB_MODEL_HPP
#define DESIGNLAB_MODEL_HPP

#include <lvgl/Runtime.hpp>
#include <lvgl/Theme.hpp>
#include <lvgl/EventBus.hpp>
#include <printer/YamlPrinter.hpp>

#include <design/extras/Form.hpp>
#include <design/ModelScope.hpp>

#include "Settings.hpp"

struct Model {
public:
  static constexpr auto worker_done_message = "Done";

  printer::YamlPrinter printer;
  lvgl::Runtime *runtime = nullptr;
  bool is_project_path_valid = false;
  bool is_theme_updated = false;
  bool is_dark_theme = false;
  bool is_export_on_startup = false;
  var::PathString new_project_path;

  SessionSettings session_settings;
  Settings project_settings;

  lvgl::Theme light_theme;
  lvgl::Theme dark_theme;

private:
  class Scope : public design::ModelScope {
  public:
    Scope() : design::ModelScope(Model::instance().model_scope_options) {}
  };
  friend Scope;
  friend class ModelInScope;
  // all access to the model must be withing a Scope

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

struct ModelInScope {
  Model::Scope scope;
public:
  Model & instance = Model::instance();
};

enum class EventBusId {
  invalid,
  notify_home,
  change_theme
};

using ApplicationEventBus = lvgl::EventBus<EventBusId>;


#endif // DESIGNLAB_MODEL_HPP
