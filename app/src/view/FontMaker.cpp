//
// Created by Tyler Gilbert on 12/12/21.
//

#include <var.hpp>

#include "designlab/fonts/FontAwesomeIcons.hpp"
#include "controller/FontManager.hpp"
#include "Editor.hpp"

#include "FontMaker.hpp"

void FontMaker::configure(Generic generic) {
  auto &editor_data = Editor::Data::create()
                        .set_add_button_text("Add Font")
                        .set_form_name(Settings::fonts_key())
                        .set_nothing_to_show("No Fonts")
                        .set_title("Fonts")
                        .set_form_title("Font Details")
                        .set_get_info_title_callback(get_info_title)
                        .set_get_feature_list_callback(get_feature_list)
                        .set_get_schema_callback(InputSchema::get_form_schema);

  generic.clear_flag(Flags::scrollable).add(Editor(editor_data).fill());
}

var::Vector<InfoCard::Data::Feature>
FontMaker::get_feature_list(json::JsonObject object) {
  var::Vector<InfoCard::Data::Feature> result;
  Settings::Font item(object);

  const auto size_list = FontManager::get_size_list(item);
  result
    .push_back(
      {.icon = icons::fa::check_solid,
       .label = "Bits Per Pixel",
       .value = item.get_bits_per_pixel()})
    .push_back(
      {.icon = icons::fa::file_alt_solid,
       .label = "File",
       .value = item.get_path()})
    .push_back(
      {.icon = icons::fa::bold_solid, .label = "Style", .value = item.get_style()})
    .push_back(
      {.icon = icons::fa::info_solid,
       .label = "Range",
       .value = item.get_range()})
    .push_back(
      {.icon = icons::fa::info_solid,
       .label = "Sizes",
       .value = size_list.string_view()})
    .push_back(
      {.icon = icons::fa::icons_solid,
       .label = "Icons",
       .value = item.is_icons() ? "Yes" : "No"});
  return result;
}

var::StringView FontMaker::get_info_title(json::JsonObject object) {
  Settings::Font item(object);
  return fs::Path::name(item.get_name());
}


FontMaker::InputSchema::InputSchema() {
  Model::Scope model_scope;
  push_back(Form::SelectFile::Schema()
              .set_name(Settings::Font::path_key())
              .set_base_path(model().session_settings.get_project())
              .set_label("Select Font File")
              .set_hint("Choose the ttf file to use."));

  push_back(Form::LineField::Schema()
              .set_name(Settings::Font::name_key())
              .set_label("Name")
              .set_value("montserrat")
              .set_hint("The name of the font (should match the file)."));
  push_back(
    Form::Select::Schema()
      .set_name(Settings::Font::style_key())
      .set_label("Style")
      .set_value("regular")
      .set_options("any\nthin\nthin_italic\nextra_light\nextra_light_"
                   "italic\nlight\nlight_italic\nregular\nregular_"
                   "italic\nmedium\nmedium_italic\nsemi_bold\nsemi_bold_"
                   "italic\nbold\nbold_italic\nextra_bold\nextra_bold_italic")
      .set_hint("The style of the font (should match the file)."));

  push_back(Form::LineField::Schema()
              .set_name(Settings::Font::range_key())
              .set_label("Range")
              .set_value("0x20-0x7F")
              .set_hint("The characters to use in ascii hex values."));

  push_back(Form::Select::Schema()
              .set_name(Settings::Font::bits_per_pixel_key())
              .set_label("Bits Per Pixel")
              .set_value("8")
              .set_options("1\n2\n4\n8")
              .set_hint("The style of the font (should match the file)."));

  push_back(Form::SectionHeadingSchema()
              .set_name(Names::sizes_section_heading)
              .set_label("Sizes"));

  push_back(Form::LineField::Schema()
              .set_name(Settings::Font::sizes_start_key())
              .set_label("Start")
              .set_value("12")
              .set_hint("The first font will be this size."));

  push_back(
    Form::LineField::Schema()
      .set_name(Settings::Font::sizes_steps_key())
      .set_label("Steps")
      .set_value("4")
      .set_hint("Use one value to evenly space the font sizes. Use comma "
                "separated values to specify each delta. e.g. 4 or "
                "4,4,4,4,8,8,8,8."));

  push_back(Form::LineField::Schema()
              .set_name(Settings::Font::sizes_total_key())
              .set_label("Total")
              .set_value("15")
              .set_hint("The number of fonts to generate. The last delta "
                        "value will be used more than once if needed."));

  push_back(Form::Switch::Schema()
              .set_name(Settings::Font::icons_key())
              .set_label("Embed Icons")
              .set_value("true")
              .set_hint("Embed the selected icons with the font."));
}


