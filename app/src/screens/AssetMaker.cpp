//
// Created by Tyler Gilbert on 12/13/21.
//

#include <var.hpp>

#include "AssetMaker.hpp"
#include "extras/Extras.hpp"

void AssetMaker::configure(Generic generic) {
  generic.clear_flag(Flags::scrollable)
    .add(Container().fill().add(Column().fill().setup([](Column column) {
      column.add(HeaderRow("Assets", Icons::plus, "Add Asset", show_form))
        .add(HorizontalLine());

      column.add(
        Paragraph("", "Add files such as images to the output binary."));

      column.add(Column(Maker::Names::show_column).fill_width().set_flex_grow());
      show_assets(column.find<Column>(Maker::Names::show_column));
    })));

  configure_form_input(generic);
  configure_form_animation(generic);
}

void AssetMaker::configure_form_input(Generic generic) {
  Model::Scope model_scope;
  generic.add(
    Container(Maker::Names::input_form_container)
      .fill()
      .add(Column(Maker::Names::input_form_column).fill().setup([](Column column) {
        column.add(FormHeaderRow("Add New Asset", hide_form))
          .add(HorizontalLine());

        model().asset_input_form_schema = InputSchema::get_form_schema();

        column.add(Form(
                     Settings::assets_key(),
                     model().asset_input_form_schema.to_object())
                     .add(AddButtonRow("Add", handle_add)));
      })));
}

void AssetMaker::handle_add(lv_event_t *e) {
  Model::Scope model_scope;
  api::ErrorScope error_scope;
  const auto form = screen().find<Form>(Settings::assets_key());
  API_ASSERT(form.is_valid());
  model().project_settings.import_form_schema(form);
  if (is_error()) {
    printer().object("error", error());
    printer().key("badInput", model().project_settings.bad_key());
  } else {
    printer().object("projectSettings", model().project_settings);
    hide_form(e);
    show_assets(screen().find<Column>(Maker::Names::show_column));
  }
}

AssetMaker::InputSchema::InputSchema() {
  Model::Scope model_scope;
  push_back(Form::SelectFile::Schema()
              .set_name(Settings::Asset::path_key())
              .set_base_path(model().session_settings.get_project())
              .set_label("Select Asset File")
              .set_hint("Choose the file to include in the binary."));
}

void AssetMaker::show_assets(Column column) {
  Model::Scope model_scope;

  column.clean();
  const auto asset_array = model().project_settings.get_assets();

  if (asset_array.count() == 0) {
    column.add(
      SubSectionHeading("No Assets Available").set_flex_grow().center());
    return;
  }

  for (const auto &asset : asset_array) {
    auto &info_card_data = InfoCard::Data::create();
    const auto name = fs::Path::name(asset.get_path());
    info_card_data.set_title(name);

    info_card_data
      .push_feature(
        {.icon = Icons::file_signature, .label = "Name", .value = name})
      .push_feature(
        {.icon = Icons::chart_area,
         .label = "Path",
         .value = asset.get_path()});

    column.add(InfoCard(info_card_data));
  }
}