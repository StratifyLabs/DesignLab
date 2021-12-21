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
  struct Names {
    static constexpr auto export_modal = "ExportModal";
    static constexpr auto title_header_row = "Exporting";
    static constexpr auto content_container = "ContentCard";
    static constexpr auto message_label = "MessageLabel";
    static constexpr auto progress_bar = "ProgressBar";
    static constexpr auto ok_button = "OkButton";
    static constexpr auto abort_button = "AbortButton";
  };


  static void ok_clicked(lv_event_t*);
  static void cancel_clicked(lv_event_t*);
  static void notified(lv_event_t*);
};

#endif // DESIGNLAB_EXPORTMODAL_HPP
