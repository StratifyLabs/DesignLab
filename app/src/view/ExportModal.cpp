//
// Created by Tyler Gilbert on 12/20/21.
//

#include <design.hpp>
#include <lvgl.hpp>

#include "extras/Extras.hpp"

#include "ExportModal.hpp"

void ExportModal::start() {
  Modal modal(Names::export_modal);

  modal.add(Container(Names::content_container)
              .set_padding(20)
              .add_style(Column::get_style())
              .set_row_padding(30)
              .add_style("card modal_content col")
              .add_style("modal_content_enabled", Modal::enabled)
              .set_height(60_percent)
              .set_width(60_percent)
              .set_alignment(Alignment::top_middle)
              .add_state(Modal::enabled)
              .set_opacity(Opacity::transparent));

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

  Model::Scope model_scope;
  if (model().export_worker.is_running()) {
    printf("Can't export right now\n");
  } else {
    model().worker_notify_object = modal.object();
    model().export_worker = ExportWorker(model().runtime);
    model().export_worker.start();
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
