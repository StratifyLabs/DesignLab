//
// Created by Tyler Gilbert on 12/8/21.
//

#include "extras/Extras.hpp"
#include "ThemeMaker.hpp"

void ThemeMaker::configure(lvgl::Generic generic) {
  generic.clear_flag(Flags::scrollable).add(Container().fill().add(Column().fill().setup([](Column column) {
    column
      .add(HeaderRow("Theme", Icons::plus, "Add Theme", show_form))
      .add(HorizontalLine());

    column.add(Paragraph("", "Add theme source files."));

    column.add(Column(Maker::Names::show_column).fill_width().set_flex_grow());
    show_themes(column.find<Column>(Maker::Names::show_column));
  })));

  configure_form_input(generic);
  configure_form_animation(generic);
}

void ThemeMaker::configure_form_input(Generic generic) {
  Model::Scope model_scope;
  generic.add(
    Container(Maker::Names::input_form_container)
      .fill()
      .add(Column(Maker::Names::input_form_column).fill().setup([](Column column) {
        column.add(FormHeaderRow("Add New Theme", hide_form))
          .add(HorizontalLine());

        model().theme_input_form_schema = InputSchema::get_form_schema();

        column.add(
          Form(Settings::themes_key(), model().theme_input_form_schema.to_object())
            .add(AddButtonRow("Add", handle_add)));
      })));
}


void ThemeMaker::handle_add(lv_event_t*e){
  Model::Scope model_scope;
  api::ErrorScope error_scope;
  const auto form = screen().find<Form>(Settings::themes_key());
  API_ASSERT(form.is_valid());
  model().project_settings.import_form_schema(form);
  if (is_error()) {
    printer().object("error", error());
    printer().key("badInput", model().project_settings.bad_key());
  } else {
    printer().object("projectSettings", model().project_settings);
    hide_form(e);
    show_themes(screen().find<Column>(Maker::Names::show_column));
  }
}


ThemeMaker::InputSchema::InputSchema() {
  Model::Scope model_scope;
  push_back(Form::SelectFile::Schema()
              .set_name(Settings::Asset::path_key())
              .set_base_path(model().session_settings.get_project())
              .set_label("Select Theme JSON File")
              .set_suffix("json")
              .set_hint("Choose the Theme configuration file."));
}

void ThemeMaker::show_themes(Column column) {
  Model::Scope model_scope;

  column.clean();
  const auto array = model().project_settings.get_themes();

  if (array.count() == 0) {
    column.add(
      SubSectionHeading("No Themes Available").set_flex_grow().center());
    return;
  }

  for (const auto &item : array) {
    auto &info_card_data = InfoCard::Data::create();
    const auto name = fs::Path::name(item.get_path());
    info_card_data.set_title(name);

    info_card_data
      .push_feature(
        {.icon = Icons::file_signature, .label = "Name", .value = name})
      .push_feature(
        {.icon = Icons::chart_area,
         .label = "Path",
         .value = item.get_path()});

    column.add(InfoCard(info_card_data));
  }
}