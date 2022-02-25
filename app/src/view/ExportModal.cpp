//
// Created by Tyler Gilbert on 12/20/21.
//

#include <fs.hpp>
#include <var.hpp>

#include "extras/Extras.hpp"

#include "logic/AssetManager.hpp"
#include "logic/FontManager.hpp"
#include "logic/ThemeManager.hpp"

#include "model/Model.hpp"

#include "ExportModal.hpp"

void ExportModal::start() {
  auto &data = Data::create(Names::export_modal);
  Modal modal(data.cast_as_name());

  modal.add_content(Container(Names::content_container)
                      .set_padding(20)
                      .add_style(Column::get_style())
                      .set_row_padding(30)
                      .add_style("card col")
                      .set_height(40_percent)
                      .set_width(60_percent));

  auto container = modal.find<Container>(Names::content_container);

  modal.add_event_callback(EventCode::notified, notified);

  container
    .add(HeaderRow(
      Names::title_header_row,
      icons::fa::times_solid,
      "Abort",
      cancel_clicked,
      "btn_danger"))
    .add(HorizontalLine())
    .add(NakedContainer().set_flex_grow())
    .add(Label(Names::message_label)
           .add_style("h3")
           .set_text_alignment(TextAlignment::center)
           .set_text_as_static("This is where the action is"))
    .add(Bar(Names::progress_bar)
           .set_width(60_percent)
           .set_value(Range().set_value(50)))
    .add(Button(Names::ok_button)
           .add_label(var::KeyString(icons::fa::check_solid).append(" OK"))
           .add_style("btn_primary")
           .add_state(State::disabled)
           .add_event_callback(EventCode::clicked, ok_clicked))
    .add(NakedContainer().set_flex_grow());

  if (data.export_worker.is_running()) {
    printf("Can't export right now\n");
  } else {
    Model::Scope ms;
    data.export_worker = ExportWorker(model().runtime);
    data.export_worker.set_associated_object(modal.object()).start();
  }
}

void ExportModal::cancel_clicked(lv_event_t *e) {
  Event(e).find_parent<Modal>(Names::export_modal).close(300_milliseconds);
}

void ExportModal::notified(lv_event_t *e) {
  Model::Scope model_scope;
  auto modal = Event(e).target<Modal>();
  modal.find<Label>(Names::message_label)
    .set_text_as_static(model().worker_message);

  modal.find<Bar>(Names::progress_bar).set_value(model().worker_progress_range);

  if (model().worker_message == Model::worker_done_message) {
    // show the OK button
    modal.find<Button>(Names::ok_button).clear_state(State::disabled);
    modal.find<HeaderRow>(Names::title_header_row)
      .get_button()
      .add_state(State::disabled);
  }
}

void ExportModal::ok_clicked(lv_event_t *e) {
  Event(e).find_parent<Modal>(Names::export_modal).close(300_milliseconds);
}

void ExportModal::ExportWorker::work() {
  const auto save_settings = [&]() {
    Model::Scope model_scope;
    auto &model = ModelAccess::model();
    // this will reload the project saving the changes
    m_project_path = model.session_settings.get_project();
    m_lv_font_path = model.session_settings.get_lv_font_conv_path();
    m_node_path = model.session_settings.get_node_path();
    const auto settings_path = Settings::get_file_path(m_project_path);
    model.project_settings.save();
    m_project_settings.copy(model.project_settings);

    model.project_settings
      = Settings(settings_path, Settings::IsOverwrite::yes);
    // grab a read-only copy
    return Settings(settings_path);
  };

  const auto settings = save_settings();

  // makes sure the output directories exist
  {
    api::ErrorScope error_scope;
    const PathString source_path = [&]() {
      Model::Scope model_scope;
      return PathString(ModelAccess::model().session_settings.get_project());
    }() / settings.get_source() / "designlab";

    FileSystem().create_directory(source_path);
    FileSystem().create_directory(source_path / "assets");
    FileSystem().create_directory(source_path / "fonts");
    FileSystem().create_directory(source_path / "themes");
    if (is_error()) {
      printf(
        "Failed to create output directories at %s\n",
        source_path.cstring());
      return;
    }
  }

  m_font_path_list = get_font_path_list();
  static constexpr auto step_count = 4;
  const auto font_count = m_font_path_list.count();
  const auto total_count = font_count + step_count;

  update_message("Exporting Assets");
  update_progress(0, total_count);
  export_assets();

  if (is_error()) {
    update_message(StringView("Asset Error: ") | error().message());
    return;
  }

  update_message("Exporting Theme");
  update_progress(1, total_count);
  export_themes();

  if (is_error()) {
    update_message(StringView("Theme Error: ") | error().message());
    return;
  }

  update_message("Exporting Fonts");
  update_progress(2, total_count);

  export_fonts();

  if (is_error()) {
    update_message(StringView("Font Error: ") | error().message());
    return;
  }

  update_progress(3 + font_count, total_count);
  update_message("Finalizing");

  export_cmake_sourcelist();
  update_progress(4 + font_count, total_count);
  update_message(Model::worker_done_message);

  save_settings();

  update_success();
}

void ExportModal::ExportWorker::update_font_progress(int value, int total) {
  update_progress(3 + value, total + 4);
}

void ExportModal::ExportWorker::export_assets() {
  API_RETURN_IF_ERROR();
  const auto options = [&]() {
    Model::Scope model_scope;
    auto &model = ModelAccess::model();

    return AssetManager::Construct{
      .input_path = "designlab.json",
      .output_path = model.project_settings.get_source(),
      .project_path = model.session_settings.get_project()};
  }();

  m_asset_path_list
    = AssetManager(options).get_source_list(m_project_path, m_project_settings);

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

void ExportModal::ExportWorker::export_themes() {
  API_RETURN_IF_ERROR();
  const auto theme_list = m_project_settings.get_themes();
  for (const auto &theme : theme_list) {
    m_theme_path_list
      = ThemeManager({.input_path = theme.get_path(),
                      .project_path = m_project_path,
                      .output_source_path = m_project_settings.get_source()})
          .get_source_list(m_project_path, m_project_settings);
    if (is_error()) {
      return;
    }
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

fs::PathList ExportModal::ExportWorker::get_font_path_list() {
  const auto options = FontManager::Construct{
    .icons = m_project_settings.icons(),
    .input_path = "designlab.json",
    .is_dry_run = true,
    .lv_font_conv_path = m_lv_font_path,
    .node_path = m_node_path,
    .output_path = m_project_settings.get_source(),
    .project_path = m_project_path};

  return FontManager(options).get_source_list(
    m_project_path,
    m_project_settings);
}

void ExportModal::ExportWorker::export_fonts() {
  API_RETURN_IF_ERROR();
  const auto options = FontManager::Construct{
    .icons = m_project_settings.icons(),
    .input_path = "designlab.json",
    .is_dry_run = !m_project_settings.is_font_dirty(),
    .lv_font_conv_path = m_lv_font_path,
    .node_path = m_node_path,
    .output_path = m_project_settings.get_source(),
    .project_path = m_project_path,
    .update_callback = update_font_progress_callback,
    .update_context = this};

  m_font_path_list
    = FontManager(options).get_source_list(m_project_path, m_project_settings);

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

void ExportModal::ExportWorker::export_cmake_sourcelist() {
  API_RETURN_IF_ERROR();
  const auto output_path = [&]() {
    Model::Scope model_scope;
    auto &model = ModelAccess::model();
    return m_project_path / m_project_settings.get_source()
           / "designlab/CMakeLists.txt";
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

void ExportModal::ExportWorker::update_message(var::StringView message) {
  m_update_message = message;
  push_task_to_runtime<ExportWorker>(this, [](ExportWorker *self) {
    auto modal = Modal(self->associated_object());
    modal.find<Label>(Names::message_label)
      .set_text(GeneralString(self->m_update_message).cstring());
  }).wait_runtime_task();
}

void ExportModal::ExportWorker::update_progress(int value, int total) {
  m_update_progress = value;
  m_update_total = total;
  push_task_to_runtime<ExportWorker>(this, [](ExportWorker *self) {
    auto modal = Modal(self->associated_object());
    modal.find<Bar>(Names::progress_bar)
      .set_value(Range()
                   .set_value(self->m_update_progress)
                   .set_maximum(self->m_update_total));
  }).wait_runtime_task();
}

void ExportModal::ExportWorker::update_success() {
  push_task_to_runtime<ExportWorker>(this, [](ExportWorker *self) {
    auto modal = Modal(self->associated_object());
    modal.find<Button>(Names::ok_button).clear_state(State::disabled);
  }).wait_runtime_task();
}
