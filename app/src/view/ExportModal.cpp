//
// Created by Tyler Gilbert on 12/20/21.
//

#include <fs.hpp>
#include <var.hpp>

#include "extras/Extras.hpp"

#include "logic/AssetManager.hpp"
#include "logic/ComponentManager.hpp"
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
    auto model = ModelInScope();
    data.export_worker = ExportWorker(model.instance.runtime, modal.object());
    data.export_worker.start();
  }
}

void ExportModal::cancel_clicked(lv_event_t *e) {
  Event(e).find_parent<Modal>(Names::export_modal).close(300_milliseconds);
}

void ExportModal::ok_clicked(lv_event_t *e) {
  Event(e).find_parent<Modal>(Names::export_modal).close(300_milliseconds);
}

void ExportModal::ExportWorker::work() {
  const auto save_settings = [&]() {
    auto model = ModelInScope();
    // this will reload the project saving the changes
    m_project_path = model.instance.session_settings.get_project();
    m_lv_font_path = model.instance.session_settings.get_lv_font_conv_path();
    m_node_path = model.instance.session_settings.get_node_path();
    const auto settings_path = Settings::get_file_path(m_project_path);
    model.instance.project_settings.save();
    m_project_settings.copy(model.instance.project_settings);
    model.instance.project_settings
      = Settings(settings_path, Settings::IsOverwrite::yes);
    // grab a read-only copy
    return Settings(settings_path);
  };

  const auto settings = save_settings();

  // makes sure the output directories exist
  {
    api::ErrorScope error_scope;
    const PathString source_path = [&]() {
      return PathString(ModelInScope().instance.session_settings.get_project());
    }() / settings.get_source() / "designlab";

    FileSystem()
      .create_directory(source_path)
      .create_directory(source_path / "assets")
      .create_directory(source_path / "fonts")
      .create_directory(source_path / "themes");
    if (is_error()) {
      printf(
        "Failed to create output directories at %s\n",
        source_path.cstring());
      return;
    }
  }

  m_font_path_list = get_font_path_list();
  static constexpr auto step_count = 5;
  const auto font_count = m_font_path_list.count();

  update_message("Exporting Theme");
  update_progress(0, 5);
  export_themes();

  if (is_error()) {
    update_message(StringView("Theme Error: ") | error().message());
    return;
  }

  const auto total_count = font_count + step_count + m_theme_font_list.count();

  update_message("Exporting Assets");
  update_progress(1, total_count);
  export_assets();

  if (is_error()) {
    update_message(StringView("Asset Error: ") | error().message());
    return;
  }

  update_message("Exporting Components");
  update_progress(2, total_count);
  export_components();

  update_message("Exporting Fonts");
  update_progress(3, total_count);

  export_fonts();

  if (is_error()) {
    update_message(StringView("Font Error: ") | error().message());
    return;
  }

  update_message("Finalizing");

  export_cmake_sourcelist();
  update_progress(total_count, total_count);
  update_message(Model::worker_done_message);

  save_settings();

  update_success();
}

void ExportModal::ExportWorker::update_font_progress(int value, int total) {
  update_progress(3 + value, total + 3);
}

void ExportModal::ExportWorker::export_assets() {
  API_RETURN_IF_ERROR();
  const auto options = [&]() {
    auto model = ModelInScope();
    return AssetManager::Construct{
      .input_path = "designlab.json",
      .output_path = model.instance.project_settings.get_source(),
      .project_path = model.instance.session_settings.get_project()};
  }();

  m_asset_path_list
    = AssetManager(options).get_source_list(m_project_path, m_project_settings);

  const auto is_assets_dirty = []() {
    auto model = ModelInScope();
    return model.instance.project_settings.is_assets_dirty();
  }();

  if (!is_assets_dirty) {
    return;
  }

  {
    auto model = ModelInScope();
    model.instance.project_settings.set_assets_dirty(false);

    model.instance.printer.info("complete");
    if (is_error()) {
      model.instance.printer.object("error", error());
    }
  }
}

void ExportModal::ExportWorker::export_themes() {
  API_RETURN_IF_ERROR();
  const auto theme_list = m_project_settings.get_themes();
  for (const auto &theme : theme_list) {
    ThemeManager theme_manager(
      {.input_path = theme.get_path(),
       .project_path = m_project_path,
       .output_source_path = m_project_settings.get_source()});
    m_theme_path_list
      = theme_manager.get_source_list(m_project_path, m_project_settings);
    const auto font_name_list = theme_manager.get_font_name_list();
    m_theme_font_list.reserve(
      m_theme_font_list.count() + font_name_list.count());
    for (const auto &font : font_name_list) {
      const auto offset = m_theme_font_list.find_offset(font);
      if (offset == m_theme_font_list.count()) {
        m_theme_font_list.push_back(font);
      }
    }
    if (is_error()) {
      return;
    }
  }

  const auto name_container
    = ThemeManager::get_name_list(m_project_path, m_project_settings);

  ThemeManager::generate_theme_source(
    m_project_path / m_project_settings.get_output_directory() / "themes",
    name_container);

  {
    auto model = ModelInScope();
    model.instance.printer.info("complete");
    if (is_error()) {
      model.instance.printer.object("error", error());
    }
  }
}

fs::PathList ExportModal::ExportWorker::get_font_path_list() {
  const FontManager::Construct options{
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
    .theme_font_list = &m_theme_font_list,
    .update_callback = update_font_progress_callback,
    .update_context = this};

  const FontManager font_manager{options};
  const auto &generated_container{font_manager.generated_container()};
  m_font_path_list.reserve(generated_container.count());
  for (const auto &source_file : generated_container) {
    const auto path = "fonts" / source_file;
    const auto offset = m_font_path_list.find_offset(path);
    if (offset == m_font_path_list.count()) {
      m_font_path_list.push_back(path);
    }
  }
  m_font_path_list.sort(fs::PathList::ascending);

  {
    auto model = ModelInScope();
    model.instance.project_settings.set_font_dirty(false);
    model.instance.printer.info("complete");
    if (is_error()) {
      model.instance.printer.object("error", error());
    }
  }
}

void ExportModal::ExportWorker::export_components() {
  const auto component_container = [&]() {
    return ModelInScope().instance.project_settings.get_components();
  }();

  const auto output_path = [&]() {
    return m_project_path / m_project_settings.get_source()
           / "designlab/components";
  }();

  fs::FileSystem().create_directory(output_path);
  API_RETURN_IF_ERROR();

  for (const auto &component : component_container) {
    ComponentManager component_manager;
    component_manager.set_component(component).generate();

    File(File::IsOverwrite::yes, output_path / component.get_name() & ".hpp")
      .write(component_manager.header_file_contents());

    File(File::IsOverwrite::yes, output_path / component.get_name() & ".cpp")
      .write(component_manager.source_file_contents());
  }
}

void ExportModal::ExportWorker::export_cmake_sourcelist() {
  API_RETURN_IF_ERROR();
  const auto output_path = [&]() {
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
  push_task_to_runtime([this]() {
    auto modal = Modal(associated_object());
    modal.find<Label>(Names::message_label)
      .set_text(GeneralString(m_update_message).cstring());
  }).wait_runtime_task();
}

void ExportModal::ExportWorker::update_progress(int value, int total) {
  m_update_progress = value;
  m_update_total = total;
  push_task_to_runtime([this]() {
    auto modal = Modal(associated_object());
    modal.find<Bar>(Names::progress_bar)
      .set_value(Range()
                   .set_value(m_update_progress)
                   .set_maximum(m_update_total));
  }).wait_runtime_task();
}

void ExportModal::ExportWorker::update_success() {
  push_task_to_runtime([this]() {
    auto modal = Modal(associated_object());
    modal.find<Button>(Names::ok_button).clear_state(State::disabled);
    {
      auto model = ModelInScope();
      if (model.instance.is_export_on_startup) {
        exit(0);
      }
    }
  }).wait_runtime_task();
}
