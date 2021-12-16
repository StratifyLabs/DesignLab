//
// Created by Tyler Gilbert on 12/16/21.
//

#ifndef DESIGNLAB_MODEL_HPP
#define DESIGNLAB_MODEL_HPP

#include <lvgl/Runtime.hpp>
#include <lvgl/Theme.hpp>
#include <printer/YamlPrinter.hpp>

#include <design/extras/Form.hpp>

#include "logic/ExportWorker.hpp"
#include "Settings.hpp"

struct Model {
public:
  API_SINGLETON_CUSTOM_CONSTRUCTOR(Model);
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

private:

  Model()
    : project_settings(
      Settings::get_file_path(session_settings.get_project()),
      Settings::IsOverwrite::yes) {}
};

class ModelAccess : public api::ExecutionContext {
public:
  static Model &model() { return Model::instance(); }
  static printer::Printer& printer(){ return model().printer; }
};

#endif // DESIGNLAB_MODEL_HPP
