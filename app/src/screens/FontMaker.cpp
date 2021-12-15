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
    })));

  configure_form_input(generic);
  configure_form_animation(generic);
}



void FontMaker::configure_form_input(Generic generic) {
  generic.add(
    Container(input_form_container_name)
      .fill()
      .add(Column(input_form_column_name).fill().setup([](Column column) {
        column.add(FormHeaderRow("Add New Font", hide_form))
          .add(HorizontalLine());

        model().font_input_form_schema = InputSchema::get_form_schema();

        column.add(
          Form(Manager::Settings::fonts_key(), model().font_input_form_schema.to_object())
            .add(AddButtonRow("Add", handle_add)));
      })));
}

void FontMaker::handle_add(lv_event_t *e) {
  api::ErrorScope error_scope;
  const auto form = screen().find<Form>(Manager::Settings::fonts_key());
  API_ASSERT(form.is_valid());
  model().project_settings.import_form_schema(form);
  if (is_error()) {
    printer().object("error", error());
    printer().key("badInput", model().project_settings.bad_key());
  } else {
    printer().object("projectSettings", model().project_settings);
    hide_form(e);
  }
}

FontMaker::InputSchema::InputSchema() {
  push_back(Form::SelectFile::Schema()
              .set_name(Manager::Settings::Font::path_key())
              .set_base_path(FileSystemWindow::root_drive_path())
              .set_label("Select Font File")
              .set_hint("Choose the ttf file to use."))
    .push_back(Form::LineField::Schema()
                 .set_name(Manager::Settings::Font::name_key())
                 .set_label("Name")
                 .set_value("montserrat")
                 .set_hint("The name of the font (should match the file)."))
    .push_back(
      Form::Select::Schema()
        .set_name(Manager::Settings::Font::style_key())
        .set_label("Style")
        .set_value("regular")
        .set_options("any\nthin\nthin_italic\nextra_light\nextra_light_"
                     "italic\nlight\nlight_italic\nregular\nregular_"
                     "italic\nmedium\nmedium_italic\nsemi_bold\nsemi_bold_"
                     "italic\nbold\nbold_italic\nextra_bold\nextra_bold_italic")
        .set_hint("The style of the font (should match the file)."))
    .push_back(Form::LineField::Schema()
                 .set_name(Manager::Settings::Font::range_key())
                 .set_label("Range")
                 .set_value("0x20-0x7F")
                 .set_hint("The characters to use in ascii hex values."))
    .push_back(Form::Select::Schema()
                 .set_name(Manager::Settings::Font::bits_per_pixel_key())
                 .set_label("Bits Per Pixel")
                 .set_value("8")
                 .set_options("1\n2\n4\n8")
                 .set_hint("The style of the font (should match the file)."))
    .push_back(Form::SectionHeadingSchema()
                 .set_name(Names::sizes_section_heading)
                 .set_label("Sizes"))
    .push_back(Form::LineField::Schema()
                 .set_name(Manager::Settings::Font::sizes_start_key())
                 .set_label("Start")
                 .set_value("12")
                 .set_hint("The first font will be this size."))
    .push_back(
      Form::LineField::Schema()
        .set_name(Manager::Settings::Font::sizes_steps_key())
        .set_label("Steps")
        .set_value("4")
        .set_hint("Use one value to evenly space the font sizes. Use comma "
                  "separated values to specify each delta. e.g. 4 or "
                  "4,4,4,4,8,8,8,8."))
    .push_back(Form::LineField::Schema()
                 .set_name(Manager::Settings::Font::sizes_total_key())
                 .set_label("Total")
                 .set_value("15")
                 .set_hint("The number of fonts to generate. The last delta "
                           "value will be used more than once if needed."));
}
