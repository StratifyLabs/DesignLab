//
// Created by Tyler Gilbert on 12/8/21.
//

#include "extras/Extras.hpp"
#include "ThemeMaker.hpp"

void ThemeMaker::configure(lvgl::Generic generic) {
  generic.add(Container().fill().add(Column().fill().setup([](Column column) {
    column
      .add(HeaderRow("Theme", Icons::plus, "Add Theme", show_form))
      .add(HorizontalLine());

    column.add(Paragraph("", "Add theme source files."));
  })));

  configure_form_input(generic);
  configure_form_animation(generic);
}

void ThemeMaker::configure_form_input(Generic generic) {
  generic.add(
    Container(input_form_container_name)
      .fill()
      .add(Column(input_form_column_name).fill().setup([](Column column) {
        column.add(FormHeaderRow("Add New Theme", hide_form))
          .add(HorizontalLine());

        model().theme_input_form_schema = InputSchema::get_form_schema();

        column.add(
          Form(Manager::Settings::fonts_key(), model().theme_input_form_schema.to_object())
            .add(AddButtonRow("Add", handle_add)));
      })));
}


void ThemeMaker::handle_add(lv_event_t*e){
  api::ErrorScope error_scope;
  const auto form = screen().find<Form>(Manager::Settings::assets_key());
  API_ASSERT(form.is_valid());
  model().project_settings.import_form_schema(form);
  if (is_error()) {
    printer().object("error", error());
    printer().key("badInput", model().project_settings.bad_key());
  } else {
    printer().object("projectSettings", model().project_settings);
    hide_form(e);
  }
}


ThemeMaker::InputSchema::InputSchema() {
  push_back(Form::SelectFile::Schema()
              .set_name(Manager::Settings::Asset::path_key())
              .set_base_path(FileSystemWindow::root_drive_path())
              .set_label("Select Theme JSON File")
              .set_hint("Choose the Theme configuration file."));
}