//
// Created by Tyler Gilbert on 12/13/21.
//

#include <var.hpp>

#include "AssetMaker.hpp"
#include "extras/Extras.hpp"

#include "Editor.hpp"

void AssetMaker::configure(Generic generic) {

  auto &editor_data = Editor::Data::create()
                        .set_add_button_text("Add Asset")
                        .set_form_name(Settings::assets_key())
                        .set_nothing_to_show("No Assets")
                        .set_title("Assets")
                        .set_form_title("Asset Details")
                        .set_get_info_title_callback(get_info_title)
                        .set_get_feature_list_callback(get_feature_list)
                        .set_validate_callback(validate)
                        .set_get_schema_callback(InputSchema::get_form_schema);

  generic.clear_flag(Flags::scrollable).add(Editor(editor_data).fill());
}

AssetMaker::InputSchema::InputSchema() {
  Model::Scope model_scope;

  push_back(Form::SelectFile::Schema()
              .set_name(Settings::Asset::path_key())
              .set_base_path(model().session_settings.get_project())
              .set_label("Select Asset File")
              .set_hint("Choose the file to include in the binary."));
}

var::Vector<InfoCard::Data::Feature>
AssetMaker::get_feature_list(json::JsonObject object) {
  var::Vector<InfoCard::Data::Feature> result;
  Settings::Asset asset(object);
  const auto name = fs::Path::name(asset.get_path());
  result
    .push_back({.icon = icons::fa::image_solid, .label = "Name", .value = name})
    .push_back(
      {.icon = icons::fa::folder_open_solid,
       .label = "Path",
       .value = asset.get_path()});
  return result;
}

var::StringView AssetMaker::get_info_title(json::JsonObject object) {
  Settings::Asset asset(object);
  return fs::Path::name(asset.get_path());
}

Editor::IsValid AssetMaker::validate(Form form) {
  auto select_file = form.find<Form::SelectFile>(Settings::Asset::path_key());
  const auto value = select_file.get_value();
  // verify the data path exists
  Model::Scope model_scope;
  const auto asset_path = model().session_settings.get_project() / value;
  if (!fs::FileSystem().exists(asset_path)) {
    select_file.set_error_message("asset does not exist");
    return IsValid::no;
  }

  select_file.hide_error_message();

  return IsValid::yes;
}
