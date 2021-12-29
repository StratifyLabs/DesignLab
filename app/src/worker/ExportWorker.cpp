//
// Created by Tyler Gilbert on 12/16/21.
//

#include <fs.hpp>
#include <var.hpp>


#include "logic/AssetManager.hpp"
#include "logic/FontManager.hpp"
#include "logic/ThemeManager.hpp"

#include "model/Model.hpp"

#include "ExportWorker.hpp"


/*
 * The constructor cannot use the model or it will
 * cause a recursive dependency with the model
 */
ExportWorker::ExportWorker(lvgl::Runtime *runtime) : WorkerObject(runtime) {}

void ExportWorker::interface_work() {
  const auto save_settings = [&]() {
    Model::Scope model_scope;
    auto &model = ModelAccess::model();
    // this will reload the project saving the changes
    m_project_path = model.session_settings.get_project();
    const auto settings_path = Settings::get_file_path(m_project_path);
    model.project_settings.save();
    model.project_settings = Settings(settings_path, Settings::IsOverwrite::yes);
    // grab a read-only copy
    return Settings(settings_path);
  };

  const auto settings = save_settings();

  //makes sure the output directories exist
  {
    api::ErrorScope error_scope;
    const PathString source_path = [&](){
      Model::Scope model_scope;
      return  PathString(ModelAccess::model().session_settings.get_project());
    }()/ settings.get_source() / "designlab";

    FileSystem().create_directory(source_path);
    FileSystem().create_directory(source_path / "assets");
    FileSystem().create_directory(source_path / "fonts");
    FileSystem().create_directory(source_path / "themes");
    if( is_error() ){
      printf("Failed to create output directories at %s\n", source_path.cstring());
      return;
    }
  }

  update_message("Exporting Assets");
  update_progress(0, 4);
  export_assets(settings);
  update_message("Exporting Theme");
  update_progress(1, 4);
  export_themes(settings);
  update_message("Exporting Fonts");
  update_progress(2, 4);
  export_fonts(settings);
  update_progress(3, 4);
  update_message("Finalizing");
  export_cmake_sourcelist(settings);
  update_progress(4, 4);
  update_message(Model::worker_done_message);
  wait_runtime_task();

  save_settings();
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

  m_asset_path_list
    = AssetManager(options).get_source_list(m_project_path, settings);

  const auto is_assets_dirty = []() {
    Model::Scope model_scope;
    return ModelAccess::model().project_settings.is_assets_dirty();
  }();

  if (!is_assets_dirty) {
    return;
  }

  {
    Model::Scope model_scope;
    auto &printer = ModelAccess::printer();
    ModelAccess::model().project_settings.set_assets_dirty(false);

    printer.info("complete");
    if (is_error()) {
      printer.object("error", error());
    }
  }
}

void ExportWorker::export_themes(const Settings &settings) {
  const auto theme_list = settings.get_themes();
  for (const auto &theme : theme_list) {
    printf("----------------------------process theme %s --> %s\n", theme.get_path_cstring(), m_project_path.cstring());
    m_theme_path_list = ThemeManager({.input_path = theme.get_path(),
                                      .project_path = m_project_path})
                          .get_source_list(m_project_path, settings);
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

void ExportWorker::export_fonts(const Settings &settings) {
  const auto options = [&]() {
    Model::Scope model_scope;
    auto &model = ModelAccess::model();

    return FontManager::Construct{
      .input_path = "designlab.json",
      .is_dry_run = !model.project_settings.is_font_dirty(),
      .output_path = model.project_settings.get_source(),
      .project_path = model.session_settings.get_project()};
  }();

  m_font_path_list
    = FontManager(options).get_source_list(m_project_path, settings);

  {
    Model::Scope model_scope;
    auto &printer = ModelAccess::printer();
    ModelAccess::model().project_settings.set_font_dirty(false);

    printer.info("complete");
    if (is_error()) {
      printer.object("error", error());
    }
  }
}

void ExportWorker::export_cmake_sourcelist(const Settings &settings) {

  const auto output_path = [&]() {
    Model::Scope model_scope;
    auto &model = ModelAccess::model();
    return m_project_path / settings.get_source() / "designlab/CMakeLists.txt";
  }();

  File file(File::IsOverwrite::yes, output_path);

  file.write("# This file was generated by DesignLab -- do not modify\n");
  file.write("set(SOURCES\n");

  auto write_list = [&](const fs::PathList &list) {
    for (const auto path : list) {
      file.write(("  " & path & "\n").string_view());
    }
  };

  write_list(m_asset_path_list);
  write_list(m_theme_path_list);
  write_list(m_font_path_list);
  file.write("  PARENT_SCOPE)\n\n");
}