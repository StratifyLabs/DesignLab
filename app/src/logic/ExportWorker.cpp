//
// Created by Tyler Gilbert on 12/16/21.
//

#include <var.hpp>

#include "AssetManager.hpp"
#include "ExportWorker.hpp"
#include "ThemeManager.hpp"

#include "model/Model.hpp"

/*
 * The constructor cannot use the model or it will
 * cause a recursive dependency with the model
 */
ExportWorker::ExportWorker(lvgl::Runtime *runtime) : WorkerObject(runtime) {}

void ExportWorker::interface_work() {
  const auto settings = [&]() {
    Model::Scope model_scope;
    auto &model = ModelAccess::model();
    // this will reload the project saving the changes
    m_project_path = model.session_settings.get_project();
    const auto settings_path = Settings::get_file_path(m_project_path);
    model.project_settings
      = Settings(settings_path, Settings::IsOverwrite::yes);

    // grab a read-only copy
    return Settings(settings_path);
  }();

  export_assets(settings);
  export_themes(settings);
}

void ExportWorker::export_assets(const Settings &settings) {
  const auto options = [&]() {
    Model::Scope model_scope;
    auto &model = ModelAccess::model();

    return AssetManager::Construct{
      .input_path = "designlab.json",
      .output_path = model.project_settings.get_source(),
      .project_path = model.session_settings.get_project()};
  }();

  (AssetManager(options));

  {
    Model::Scope model_scope;
    auto &printer = ModelAccess::printer();

    printer.info("complete");
    if (is_error()) {
      printer.object("error", error());
    }
  }
}

void ExportWorker::export_themes(const Settings &settings) {
  const auto theme_list = settings.get_themes();
  for (const auto &theme : theme_list) {
    ThemeManager(
      {.input_path = theme.get_path(), .project_path = m_project_path});
  }

  {
    Model::Scope model_scope;
    auto &printer = ModelAccess::printer();

    printer.info("complete");
    if (is_error()) {
      printer.object("error", error());
    }
  }
}
