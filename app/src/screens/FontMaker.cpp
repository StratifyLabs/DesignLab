//
// Created by Tyler Gilbert on 12/12/21.
//

#include "FontMaker.hpp"

void FontMaker::configure(Generic generic) {
  generic.add(Container().fill().add(Column().fill().setup([](Column column) {
    column.add(ScreenHeading("Fonts")).add(HorizontalLine());

    column.add(Form().setup(configure_form_input));
  })));
}

void FontMaker::configure_form_input(Form form) {

  static constexpr auto select_file_name = "SelectFile";
  static constexpr auto range_name = "Range";
  static constexpr auto bits_per_pixel_name = "Bpp";
  static constexpr auto sizes_name = "sizes";

  form
    .add(Form::SelectFile(Form::SelectFile::Data::create(select_file_name)
                            .set_base_path("/")
                            .set_select_file(true)))
    .add(Form::LineField(range_name)
           .set_label_as_static("Range")
           .set_value("0x20-0x7f"))
    .add(Form::LineField(bits_per_pixel_name)
           .set_label_as_static("Bits Per Pixel")
           .set_value("8"))
    .add(Form::LineField(sizes_name)
           .set_label_as_static("Sizes")
           .set_value("8,10,12,14,16,18,20,22,24,26,28,30,32,34,36,40,44,48,52,"
                      "56,60,64,72,80"));
}
