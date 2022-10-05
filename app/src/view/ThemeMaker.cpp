//
// Created by Tyler Gilbert on 12/8/21.
//

#include "ThemeMaker.hpp"
#include "Editor.hpp"
#include "extras/Extras.hpp"

using namespace design;

namespace {
class InputSchema : var::Vector<json::JsonObject> {
public:
  InputSchema() {
    push_back(
      Form::SelectFile::Schema()
        .set_name(Settings::Theme::path_key())
        .set_base_path(ModelInScope().instance.session_settings.get_project())
        .set_label("Select Theme JSON File")
        .set_suffix("json")
        .set_hint("Choose the Theme configuration file."));
  }

  static Form::Schema get_form_schema() {
    Form::Schema result;
    result.set_name(Settings::fonts_key())
      .set_type(Form::Schema::schema_type)
      .set_input(InputSchema());
    return result;
  }
};

var::Vector<InfoCard::Data::Feature> get_feature_list(json::JsonObject object) {
  var::Vector<InfoCard::Data::Feature> result;
  Settings::Theme theme(object);
  result.push_back(
    {.icon = icons::fa::folder_open_solid,
     .label = "Path",
     .value = theme.get_path()});
  return result;
}

var::StringView get_info_title(json::JsonObject object) {
  Settings::Theme theme(object);
  return fs::Path::name(theme.get_path());
}

} // namespace

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
