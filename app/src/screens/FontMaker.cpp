//
// Created by Tyler Gilbert on 12/12/21.
//

#include <var.hpp>

#include "extras/Extras.hpp"

#include "FontMaker.hpp"

void FontMaker::configure(Generic generic) {

  generic.clear_flag(Flags::scrollable)
    .add(Container().fill().add(Column().fill().setup([](Column column) {
      column.add(HeaderRow("Fonts", Icons::plus, "Add Font", show_form))
        .add(HorizontalLine());

      column.add(
        Column(Maker::Names::show_column).fill_width().set_flex_grow());
      show_fonts(column.find<Column>(Maker::Names::show_column));
    })));

  configure_form_input(generic);
  configure_form_animation(generic);
}

void FontMaker::show_fonts(Column column) {
  Model::Scope model_scope;

  column.clean();
  const auto font_array = model().project_settings.get_fonts();

  if (font_array.count() == 0) {
    column.add(
      SubSectionHeading("No Fonts Available").set_flex_grow().center());
    return;
  }

  for (const auto &font : font_array) {
    auto &info_card_data = InfoCard::Data::create();

    const auto title = KeyString(font.get_name());
    info_card_data.set_title(title);

    const auto size_list = [&]() {
      auto start = font.get_sizes_start().to_integer();
      const auto step_list = font.get_sizes_steps().split(",");
      const auto total = font.get_sizes_total().to_integer();
      KeyString result;

      int step = 0;
      for (const auto i : api::Index(total)) {
        result.append(NumberString(start, "%d,").string_view());
        if (i < step_list.count()) {
          step = step_list.at(i).to_integer();
        }
        start += step;
      }
      return result.pop_back();
    }();

    info_card_data
      .push_feature(
        {.icon = Icons::memory,
         .label = "Bits Per Pixel",
         .value = font.get_bits_per_pixel()})
      .push_feature(
        {.icon = Icons::file_signature,
         .label = "File",
         .value = font.get_path()})
      .push_feature(
        {.icon = Icons::stream, .label = "Style", .value = font.get_style()})
      .push_feature(
        {.icon = Icons::chart_area,
         .label = "Range",
         .value = font.get_range()})
      .push_feature(
        {.icon = Icons::exchange_alt, .label = "Sizes", .value = size_list});

    column.add(InfoCard(info_card_data));
  }
}

void FontMaker::configure_form_input(Generic generic) {
  Model::Scope model_scope;
  generic.add(
    Container(Maker::Names::input_form_container)
      .fill()
      .add(
        Column(Maker::Names::input_form_column).fill().setup([](Column column) {
          column.add(FormHeaderRow("Add New Font", hide_form))
            .add(HorizontalLine());

          model().font_input_form_schema = InputSchema::get_form_schema();

          column.add(Form(
                       Settings::fonts_key(),
                       model().font_input_form_schema.to_object())
                       .add(AddButtonRow("Add", handle_add)));
        })));
}

void FontMaker::handle_add(lv_event_t *e) {
  Model::Scope model_scope;
  api::ErrorScope error_scope;
  const auto form = screen().find<Form>(Settings::fonts_key());
  API_ASSERT(form.is_valid());
  model().project_settings.import_form_schema(form);
  if (is_error()) {
    printer().object("error", error());
    printer().key("badInput", model().project_settings.bad_key());
  } else {
    printer().object("projectSettings", model().project_settings);
    hide_form(e);
    show_fonts(screen().find<Column>(Maker::Names::show_column));
  }
}

FontMaker::InputSchema::InputSchema() {
  Model::Scope model_scope;
  push_back(Form::SelectFile::Schema()
              .set_name(Settings::Font::path_key())
              .set_base_path(model().session_settings.get_project())
              .set_label("Select Font File")
              .set_hint("Choose the ttf file to use."))
    .push_back(Form::LineField::Schema()
                 .set_name(Settings::Font::name_key())
                 .set_label("Name")
                 .set_value("montserrat")
                 .set_hint("The name of the font (should match the file)."))
    .push_back(
      Form::Select::Schema()
        .set_name(Settings::Font::style_key())
        .set_label("Style")
        .set_value("regular")
        .set_options("any\nthin\nthin_italic\nextra_light\nextra_light_"
                     "italic\nlight\nlight_italic\nregular\nregular_"
                     "italic\nmedium\nmedium_italic\nsemi_bold\nsemi_bold_"
                     "italic\nbold\nbold_italic\nextra_bold\nextra_bold_italic")
        .set_hint("The style of the font (should match the file)."))
    .push_back(Form::LineField::Schema()
                 .set_name(Settings::Font::range_key())
                 .set_label("Range")
                 .set_value("0x20-0x7F")
                 .set_hint("The characters to use in ascii hex values."))
    .push_back(Form::Select::Schema()
                 .set_name(Settings::Font::bits_per_pixel_key())
                 .set_label("Bits Per Pixel")
                 .set_value("8")
                 .set_options("1\n2\n4\n8")
                 .set_hint("The style of the font (should match the file)."))
    .push_back(Form::SectionHeadingSchema()
                 .set_name(Names::sizes_section_heading)
                 .set_label("Sizes"))
    .push_back(Form::LineField::Schema()
                 .set_name(Settings::Font::sizes_start_key())
                 .set_label("Start")
                 .set_value("12")
                 .set_hint("The first font will be this size."))
    .push_back(
      Form::LineField::Schema()
        .set_name(Settings::Font::sizes_steps_key())
        .set_label("Steps")
        .set_value("4")
        .set_hint("Use one value to evenly space the font sizes. Use comma "
                  "separated values to specify each delta. e.g. 4 or "
                  "4,4,4,4,8,8,8,8."))
    .push_back(Form::LineField::Schema()
                 .set_name(Settings::Font::sizes_total_key())
                 .set_label("Total")
                 .set_value("15")
                 .set_hint("The number of fonts to generate. The last delta "
                           "value will be used more than once if needed."));
}
