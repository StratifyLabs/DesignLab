//
// Created by Tyler Gilbert on 12/16/21.
//

#include <fs.hpp>
#include <sys.hpp>
#include <var.hpp>

#include <lvgl/AssetFile.hpp>

#include "designlab/fonts/FontAwesomeIcons.hpp"

#include "FontManager.hpp"

FontManager::FontManager(const Construct &options) {

  const auto program_path = Process::which("lv_font_conv");
  if (program_path.is_empty()) {
    API_RETURN_ASSIGN_ERROR("`lv_font_conv` not found on the path", EINVAL);
  }

  const auto input_object
    = load_json_file(options.project_path / options.input_path);
  API_RETURN_IF_ERROR();

  static constexpr auto font_solid_name
    = "Font Awesome 5 Free-Solid-900.otf";
  static constexpr auto font_regular_name
    = "Font Awesome 5 Free-Regular-400.otf";
  static constexpr auto font_brands_name = "Font Awesome 5 Brands-Regular-400.otf";
  static constexpr auto font_file_list
    = {font_solid_name, font_regular_name, font_brands_name};

  for (const auto *font : font_file_list) {
    const auto path = get_temporary_font_path(font);
    if (!FileSystem().exists(path)) {
      File(File::IsOverwrite::yes, path)
        .write(lvgl::AssetFile("a:" | StringView(font)));
    }
  }

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

      if (font.is_icons()) {
        Model::Scope model_scope;
        auto add_icon_range = [&](var::StringView font_name, var::StringView family){
          const auto icon_range
            = get_icon_font_range(model().project_settings.icons(), family);
          if( icon_range.length() ) {
            arguments.push("--font=" | get_temporary_font_path(font_name))
              .push(
                "--range="
                | icon_range);
          }
        };

        add_icon_range(font_solid_name, "solid");
        add_icon_range(font_regular_name, "regular");
        add_icon_range(font_brands_name, "brands");
      }

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

  const auto full_output_directory = options.project_path / output_directory;
#if USE_BOOTSTRAP_ICONS
  generate_bootstrap_icons_hpp(full_output_directory);
#endif
  generate_fontawesome_icons_hpp(full_output_directory);
  generate_fonts_source(full_output_directory, font_list);
}

var::PathString FontManager::get_temporary_font_path(var::StringView filename) {
  return SessionSettings::get_application_directory() / filename;
}

fs::PathList
FontManager::get_source_list(var::StringView, const Settings &settings) {
  fs::PathList result;
  result.push_back("fonts/fonts.c").push_back("fonts/fonts.h");
  const auto font_list = settings.get_fonts();
  for (const auto &font : font_list) {
    const auto sizes = get_size_list(font);
    for (const auto font_size : sizes.string_view().split(",")) {
      result.push_back("fonts" / get_file_name(font, font_size));
    }
  }
  return result;
}

var::String FontManager::get_icon_font_range(
  Settings::Icons icons,
  var::StringView family) {
  var::String result;
  const auto range = icons.get_range();
  for (const auto &item : range) {
    if (item.get_family() == family) {
      result += item.get_unicode() + ",";
    }
  }
  return result.pop_back().replace(
    var::String::Replace().set_old_string("u").set_new_string("0x"));
}

void FontManager::generate_bootstrap_icons_hpp(
  const var::StringView directory) {
  // generate the Icons.hpp file
  const auto icon_hpp_path = directory / "BootstrapIcons.hpp";
  File icon_hpp(File::IsOverwrite::yes, icon_hpp_path);
  CodePrinter code_printer(icon_hpp);

  {
    Model::Scope model_scope;
    code_printer.inline_comment("This is a generated source file, do not edit")
      .newline();
    cprinter::CppPrinter::HeaderGuard header_guard(
      code_printer,
      "BOOTSTRAP_ICONS_HPP");
    cprinter::CppPrinter::NamespaceScope icons_namespace(code_printer, "icons");
    cprinter::CppPrinter::NamespaceScope fa_namespace(code_printer, "bs");
    const auto icons = model().project_settings.icons();
    for (const auto &icon : icons.get_range()) {
      if (icon.get_name() == "bootstrap") {
        const auto needs_x_prefix = [&]() {
          if (icon.get_name().length()) {
            const auto first = icon.get_name().at(0);
            return first >= '0' && first <= '9';
          }
          return false;
        }();

        const auto name
          = String((needs_x_prefix ? "x" : "") + icon.get_name())
              .replace(
                String::Replace().set_old_string("-").set_new_string("_"));

        code_printer.statement(
          "static constexpr auto " | name.string_view() | " = \"\\"
          | icon.get_unicode() | "\"");
      }
    }
  }
}

void FontManager::generate_fontawesome_icons_hpp(var::StringView directory) {
  const auto icon_hpp_path = directory / "FontAwesomeIcons.hpp";
  File icon_hpp(File::IsOverwrite::yes, icon_hpp_path);
  CodePrinter code_printer(icon_hpp);

  {
    Model::Scope model_scope;
    code_printer.inline_comment("This is a generated source file, do not edit")
      .newline();
    cprinter::CppPrinter::HeaderGuard header_guard(
      code_printer,
      "FONTAWESOME_ICONS_HPP");
    cprinter::CppPrinter::NamespaceScope icons_namespace(code_printer, "icons");
    cprinter::CppPrinter::NamespaceScope fa_namespace(code_printer, "fa");
    const auto icons = model().project_settings.icons();
    for (const auto &icon : icons.get_range()) {
      if (icon.get_name() != "bootstrap") {
        const auto needs_x_prefix = [&]() {
          if (icon.get_name().length()) {
            const auto first = icon.get_name().at(0);
            return first >= '0' && first <= '9';
          }
          return false;
        }();

        const auto name
          = String(
              (needs_x_prefix ? "x" : "") + icon.get_name() + "_"
              + icon.get_family())
              .replace(
                String::Replace().set_old_string("-").set_new_string("_"));

        code_printer.statement(
          "static constexpr auto " | name.string_view() | " = \"\\"
          | icon.get_unicode() | "\"");
      }
    }
  }
}

void FontManager::generate_fonts_source(
  var::StringView directory,
  const var::Vector<Settings::Font> &font_list) {

  File fonts_h(File::IsOverwrite::yes, directory / "fonts.h");
  File fonts_c(File::IsOverwrite::yes, directory / "fonts.c");

  CodePrinter h_printer(fonts_h);
  CodePrinter c_printer(fonts_c);

  {
    CodePrinter::HeaderGuard header_guard(h_printer, "DESIGNLAB_FONTS_H_");
    h_printer.newline();
    h_printer.line("#if defined __cplusplus")
      .line("extern \"C\" {")
      .line("#endif")
      .newline()
      .line("void fonts_initialize();")
      .newline()
      .line("#if defined __cplusplus")
      .line("}")
      .line("#endif")
      .newline();
  }

  c_printer.newline().newline();

  c_printer.line("#include \"fonts.h\"")
    .line("#include <lvgl.h>")
    .line("#include <lvgl_api.h>")
    .newline()
    .newline();

  for_each_font<CodePrinter>(
    font_list,
    c_printer,
    [](
      const Settings::Font &font,
      var::StringView font_size,
      CodePrinter &c_printer) {
      c_printer.statement(
        "extern const lv_font_t " | get_file_name(font, font_size).pop_back(2));
    });

  c_printer.newline().newline();

  {
    CodePrinter::StructInitialization struct_init(
      c_printer,
      "static const lvgl_api_font_descriptor_t lvgl_font_list[]");

    for_each_font<CodePrinter::StructInitialization>(
      font_list,
      struct_init,
      [](
        const Settings::Font &font,
        var::StringView font_size,
        CodePrinter::StructInitialization &struct_init) {
        const auto variable_name = get_file_name(font, font_size).pop_back(2);
        const auto text_name
          = PathString(variable_name)
              .replace(
                PathString::Replace().set_old_character('_').set_new_character(
                  '-'));

        struct_init.add_member(
          "{ .name = \"" | text_name | "\", .font = &" | variable_name | "}");
      });
  }

  c_printer.newline().newline();
  {
    CodePrinter::Function get_font_function(
      c_printer,
      "static const lvgl_api_font_descriptor_t *get_font(int offset)");

    c_printer.statement(
      "const int count = sizeof(lvgl_font_list) / "
      "sizeof(lvgl_api_font_descriptor_t)");

    {
      CodePrinter::IfScope check_offset(
        c_printer,
        "offset >= 0 && offset < count");
      c_printer.statement("return lvgl_font_list + offset");
    }
    c_printer.statement("return NULL");
  }

  {
    CodePrinter::Function get_font_function(
      c_printer,
      "void fonts_initialize()");
    c_printer.statement("lvgl_api_set_font_callback(get_font)");
  }
}