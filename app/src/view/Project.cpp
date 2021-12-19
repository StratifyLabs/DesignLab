//
// Created by Tyler Gilbert on 12/15/21.
//

#include "extras/Extras.hpp"

#include "controller/ExportWorker.hpp"

#include "Project.hpp"

void Project::configure(Generic generic) {
  generic.clear_flag(Flags::scrollable)
    .add(Container(ViewObject::Names::content_container)
           .add_event_callback(EventCode::exited, handle_exited)
           .fill()
           .add(Column(Names::project_column)));

  auto column = generic.find<Column>(Names::project_column);
  column.fill()
    .add(
      HeaderRow("Project", icons::fa::external_link_alt_solid, "Export", export_project))
    .add(HorizontalLine())
    .add(Paragraph("", "Add theme source files."))
    .add(Form(Names::project_form).setup(configure_form));
}

void Project::handle_exited(lv_event_t *) {
  Model::Scope model_scope;
  model().session_settings.set_project(
    screen().find<Form::SelectFile>(Names::directory_select_file).get_value());

  model().project_settings.set_source(
    screen().find<Form::SelectFile>(Names::source_select_file).get_value());
}

void Project::export_project(lv_event_t *) {
  Model::Scope model_scope;
  if (model().export_worker.is_running()) {
    printf("Can't export right now\n");
  } else {
    model().export_worker = ExportWorker(model().runtime);
    model().export_worker.start();
  }
}

void Project::configure_form(Form form) {
  Model::Scope model_scope;
  form.add(
    Form::SelectFile(
      Form::SelectFile::Data::create(Names::directory_select_file)
        .set_select_folder()
        .set_absolute_path())
      .set_value(model().session_settings.get_project_cstring())
      .set_label("Select Project Directory")
      .set_hint("A file called `designlab.json` will be created in the project "
                "directory if it doesn't already exist."));

  printf(
    "Starting source path is %s\n",
    model().project_settings.get_source_cstring());
  form.add(
    Form::SelectFile(
      Form::SelectFile::Data::create(Names::source_select_file)
        .set_select_folder()
        .set_base_path(screen()
                         .find<Form::SelectFile>(Names::directory_select_file)
                         .get_value()))
      .set_value(model().project_settings.get_source_cstring())
      .set_label("Specify Source Directory")
      .set_hint("The source directory where the `designlab/*.c` files will be "
                "generated. This is relative to the project directory."));
}
