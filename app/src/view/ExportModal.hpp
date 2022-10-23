//
// Created by Tyler Gilbert on 12/20/21.
//

#ifndef DESIGNLAB_EXPORTMODAL_HPP
#define DESIGNLAB_EXPORTMODAL_HPP

#include "ViewObject.hpp"

class ExportModal : public ViewObject {
public:
  static void start();

private:
  class ExportWorker : public WorkerAccess<ExportWorker> {
  public:
    ExportWorker() = default;
    explicit ExportWorker(Runtime *runtime, lv_obj_t * object)
      : WorkerAccess<ExportWorker>(runtime, object) {}

  private:
    var::StringView m_update_message;
    int m_update_progress;
    int m_update_total;

    var::PathString m_project_path;
    var::PathString m_lv_font_path;
    var::PathString m_node_path;
    Settings m_project_settings;
    fs::PathList m_asset_path_list;
    fs::PathList m_theme_path_list;
    fs::PathList m_font_path_list;

    fs::PathList m_theme_font_list;

    void work() override;

    void update_message(var::StringView message);
    void update_progress(int value, int total);
    void update_success();

    void export_assets();
    void export_themes();
    fs::PathList get_font_path_list();
    void export_fonts();
    void export_components();
    void export_cmake_sourcelist();

    static void
    update_font_progress_callback(void *context, int value, int total) {
      reinterpret_cast<ExportWorker *>(context)->update_font_progress(
        value,
        total);
    }

    void update_font_progress(int value, int total);
  };

  struct Data : public UserDataAccess<Data> {
    Data(const char *name) : UserDataAccess<Data>(name) {}

    ExportWorker export_worker;
  };

  struct Names {
    static constexpr auto export_modal = "ExportModal";
    static constexpr auto title_header_row = "Exporting";
    static constexpr auto content_container = "ContentCard";
    static constexpr auto message_label = "MessageLabel";
    static constexpr auto progress_bar = "ProgressBar";
    static constexpr auto ok_button = "OkButton";
    static constexpr auto abort_button = "AbortButton";
  };

  static void ok_clicked(lv_event_t *);
  static void cancel_clicked(lv_event_t *);
};

#endif // DESIGNLAB_EXPORTMODAL_HPP
