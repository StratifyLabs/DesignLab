//
// Created by Tyler Gilbert on 12/13/21.
//

#include "AssetMaker.hpp"
#include "extras/Extras.hpp"

#include "Editor.hpp"

using namespace var;

namespace {
using IsValid = Editor::IsValid;
class InputSchema : var::Vector<json::JsonObject> {
public:
  InputSchema() {
    push_back(
      Form::SelectFile::Schema()
        .set_name(Settings::Asset::path_key())
        .set_base_path(ModelInScope().instance.session_settings.get_project())
        .set_label("Select Asset File")
        .set_hint("Choose the file to include in the binary."));
  }

  static Form::Schema get_form_schema() {
    Form::Schema result;
    result.set_name(Settings::assets_key())
      .set_type(Form::Schema::schema_type)
      .set_input(InputSchema());
    return result;
  }
};

var::Vector<InfoCard::Data::Feature> get_feature_list(json::JsonObject object) {
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

var::StringView get_info_title(json::JsonObject object) {
  Settings::Asset asset(object);
  return fs::Path::name(asset.get_path());
}

Editor::IsValid validate(Form form) {
  auto select_file = form.find<Form::SelectFile>(Settings::Asset::path_key());
  const auto value = select_file.get_value();
  // verify the data path exists
  const auto asset_path
    = ModelInScope().instance.session_settings.get_project() / value;
  if (!fs::FileSystem().exists(asset_path)) {
    select_file.set_error_message("asset does not exist");
    return IsValid::no;
  }

  select_file.hide_error_message();
  return IsValid::yes;
}
} // namespace

AssetMaker::AssetMaker(const char *name) {
  construct_object(name);
  fill();
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
  clear_flag(Flags::scrollable).add(Editor(editor_data).fill());
}
