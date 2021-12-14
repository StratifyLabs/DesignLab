//
// Created by Tyler Gilbert on 12/12/21.
//

#include "FontMaker.hpp"

void FontMaker::configure(Generic generic) {
  generic.add(Container().fill().add(Column().fill().setup([](Column column) {
    column.add(ScreenHeading("Fonts")).add(HorizontalLine());

    static auto form_schema = InputSchema::get_form_schema();
    column.add(Form("", form_schema.to_object()));
  })));
}

void FontMaker::configure_form_input(Form form) {

  static constexpr auto select_file_name = "Select Font File";
  static constexpr auto name_name = "Name";
  static constexpr auto style_name = "Style";
  static constexpr auto range_name = "Range";
  static constexpr auto bits_per_pixel_name = "Bpp";
  static constexpr auto sizes_name = "sizes";
  static constexpr auto sizes_start_name = "sizesStart";
  static constexpr auto sizes_delta_name = "sizesDelta";
  static constexpr auto sizes_count_name = "sizesCount";

  form
    .add(Form::SelectFile(Form::SelectFile::Data::create(select_file_name)
                            .set_base_path("/")
                            .set_select_file(true))
           .set_label_as_static("Select Font File")
           .set_hint_as_static("Choose the ttf file to use."))
    .add(Form::LineField(name_name)
           .set_label_as_static("Name")
           .set_value("montserrat")
           .set_hint_as_static("The name of the font (should match the file)."))
    .add(
      Form::Select(style_name)
        .set_label_as_static("Style")
        .setup([](Form::Select dropdown) {
          dropdown.get_dropdown().set_options_as_static(
            "any\nthin\nthin_italic\nextra_light\nextra_light_"
            "italic\nlight\nlight_italic\nregular\nregular_"
            "italic\nmedium\nmedium_italic\nsemi_bold\nsemi_bold_"
            "italic\nbold\nbold_italic\nextra_bold\nextra_bold_italic\n");
        })
        .set_hint_as_static("The style of the font (should match the file)."))
    .add(Form::LineField(range_name)
           .set_label_as_static("Range")
           .set_value("0x20-0x7f")
           .set_hint_as_static(
             "Specify the characters to use in ascii hex values."))
    .add(Form::Select(bits_per_pixel_name)
           .set_label_as_static("Bits Per Pixel")
           .setup([](Form::Select dropdown) {
             dropdown.get_dropdown().set_options_as_static("1\n2\n4\n8");
           }))
    .add(SubSectionHeading("Sizes"))
    .add(Form::LineField(sizes_start_name)
           .set_label_as_static("Start")
           .set_value("12")
           .set_hint_as_static("The first font will be this size."))
    .add(
      Form::LineField(sizes_delta_name)
        .set_label_as_static("Delta")
        .set_value("4")
        .set_hint_as_static(
          "Use one value to evenly space the font sizes. Use comma "
          "separated values to specify each delta. e.g. 4 or 4,4,4,4,8,8,8,8."))
    .add(
      Form::LineField(sizes_count_name)
        .set_label_as_static("Count")
        .set_value("15")
        .set_hint_as_static("The number of fonts to generate. The last delta "
                            "value will be used more than once if needed."));
}

FontMaker::InputSchema::InputSchema() {
  push_back(Form::SelectFile::Schema()
              .set_name(Names::select_file)
              .set_base_path(FileSystemWindow::root_drive_path())
              .set_hint("Choose the ttf file to use."))
    .push_back(Form::LineField::Schema()
                 .set_name(Names::font_name)
                 .set_label("Name")
                 .set_value("montserrat")
                 .set_hint("The name of the font (should match the file)."))
    .push_back(Form::Select::Schema()
                 .set_name(Names::font_style)
                 .set_label("Style")
                 .set_value("regular")
                 .set_options(
                   "any\nthin\nthin_italic\nextra_light\nextra_light_"
                   "italic\nlight\nlight_italic\nregular\nregular_"
                   "italic\nmedium\nmedium_italic\nsemi_bold\nsemi_bold_"
                   "italic\nbold\nbold_italic\nextra_bold\nextra_bold_italic\n")
                 .set_hint("The style of the font (should match the file)."))
    .push_back(Form::LineField::Schema()
                 .set_name(Names::character_range)
                 .set_label("Range")
                 .set_value("0x20-0x7F")
                 .set_hint("The characters to use in ascii hex values."))
    .push_back(Form::Select::Schema()
                 .set_name(Names::bits_per_pixel)
                 .set_label("Bits Per Pixel")
                 .set_value("8")
                 .set_options("1\n2\n4\n8\n")
                 .set_hint("The style of the font (should match the file)."))
    .push_back(Form::SectionHeadingSchema()
                 .set_name(Names::sizes_section_heading)
                 .set_label("Sizes"))
    .push_back(Form::LineField::Schema()
                 .set_name(Names::sizes_start)
                 .set_label("Start")
                 .set_value("12")
                 .set_hint("The first font will be this size."))
    .push_back(
      Form::LineField::Schema()
        .set_name(Names::sizes_steps)
        .set_label("Steps")
        .set_value("4")
        .set_hint("Use one value to evenly space the font sizes. Use comma "
                  "separated values to specify each delta. e.g. 4 or "
                  "4,4,4,4,8,8,8,8."))
    .push_back(Form::LineField::Schema()
                 .set_name(Names::sizes_total)
                 .set_label("Total")
                 .set_value("15")
                 .set_hint("The number of fonts to generate. The last delta "
                           "value will be used more than once if needed."));
}
