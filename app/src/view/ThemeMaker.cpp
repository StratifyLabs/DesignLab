//
// Created by Tyler Gilbert on 12/8/21.
//

#include "ThemeMaker.hpp"
#include "Editor.hpp"
#include "extras/Extras.hpp"

ThemeMaker::ThemeMaker(const char *name) {
  construct_object(name);
  fill();

  auto &editor_data = Editor::Data::create()
                        .set_add_button_text("Add Theme")
                        .set_form_name(Settings::themes_key())
                        .set_nothing_to_show("No Themes")
                        .set_title("Themes")
                        .set_form_title("Theme Details")
                        .set_get_info_title_callback(get_info_title)
                        .set_get_feature_list_callback(get_feature_list)
                        .set_get_schema_callback(InputSchema::get_form_schema);

  clear_flag(Flags::scrollable).add(Editor(editor_data).fill());
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
  Settings::Theme theme(object);
  const auto name = fs::Path::name(theme.get_path());
  result
    .push_back(
      {.icon = icons::fa::th_list_solid, .label = "Name", .value = name})
    .push_back(
      {.icon = icons::fa::folder_open_solid,
       .label = "Path",
       .value = theme.get_path()});
  return result;
}

var::StringView ThemeMaker::get_info_title(json::JsonObject object) {
  Settings::Theme theme(object);
  return fs::Path::name(theme.get_path());
}
