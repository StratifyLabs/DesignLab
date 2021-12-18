//
// Created by Tyler Gilbert on 12/16/21.
//

#include <sys.hpp>

#include "FontManager.hpp"

FontManager::FontManager(const Construct &options) {

  const auto program_path = Process::which("lv_font_conv");
  if (program_path.is_empty()) {
    API_RETURN_ASSIGN_ERROR("`lv_font_conv` not found on the path", EINVAL);
  }

  const auto input_object
    = load_json_file(options.project_path / options.input_path);
  API_RETURN_IF_ERROR();

  const auto settings = Settings(input_object);
  const auto font_list = settings.get_fonts();
  const auto output_directory = settings.get_output_directory() / "fonts";

  for (const auto &font : font_list) {

    const auto sizes = get_size_list(font);

    for (const auto font_size : sizes.string_view().split(",")) {
      var::GeneralString command = "lv_font_conv";

      const auto output_file_path
        = output_directory / get_file_name(font, font_size);

      Process::Arguments arguments(program_path);
      arguments.push("--bpp=" | font.get_bits_per_pixel())
        .push("--size=" | font_size)
        .push("--format=lvgl")
        .push("--output=" | output_file_path.string_view())
        .push("--font=" | font.get_path())
        .push("--range=" | font.get_range());

      {
        Model::Scope model_scope;
        auto &printer = ModelAccess::printer();
        for (const auto arg : arguments.arguments()) {
          if (arg != nullptr) {
            printer.info(arg);
          }
        }
      }

#if 1
      Process lv_font_conv(
        arguments,
        Process::Environment().set_working_directory(options.project_path));
      lv_font_conv.wait();
      {
        Model::Scope model_scope;
        auto &printer = ModelAccess::printer();
        printer.key(
          "result",
          var::NumberString(lv_font_conv.status().exit_status()));
      }
#endif

      // was there an error?
    }
  }
}

fs::PathList FontManager::get_source_list(var::StringView, const Settings &settings) {
  fs::PathList result;
  const auto font_list = settings.get_fonts();
  for (const auto &font : font_list) {
    const auto sizes = get_size_list(font);
    for (const auto font_size : sizes.string_view().split(",")) {
      result.push_back("fonts" / get_file_name(font, font_size));
    }
  }
  return result;
}
