//
// Created by Tyler Gilbert on 12/8/21.
//

#include "ThemeMaker.hpp"
#include "extras/Extras.hpp"
#include "Editor.hpp"

void ThemeMaker::configure(lvgl::Generic generic) {

    auto &editor_data = Editor::Data::create()
                          .set_add_button_text("Add Theme")
                          .set_form_name(Settings::themes_key())
                          .set_nothing_to_show("No Themes")
                          .set_title("Themes")
                          .set_form_title("Theme Details")
                          .set_get_info_title_callback(get_info_title)
                          .set_get_feature_list_callback(get_feature_list)
                          .set_get_schema_callback(InputSchema::get_form_schema);

    generic.clear_flag(Flags::scrollable).add(Editor(editor_data).fill());
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

var::Vector<InfoCard::Data::Feature>
ThemeMaker::get_feature_list(json::JsonObject object) {
  var::Vector<InfoCard::Data::Feature> result;
  Settings::Asset asset(object);
  const auto name = fs::Path::name(asset.get_path());
  result
    .push_back({.icon = Icons::file_signature, .label = "Name", .value = name})
    .push_back(
      {.icon = Icons::chart_area, .label = "Path", .value = asset.get_path()});
  return result;
}

var::StringView ThemeMaker::get_info_title(json::JsonObject object) {
  Settings::Theme theme(object);
  return fs::Path::name(theme.get_path());
}