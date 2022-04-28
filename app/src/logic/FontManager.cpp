//
// Created by Tyler Gilbert on 12/16/21.
//

#include <fs.hpp>
#include <sys.hpp>
#include <var.hpp>

#include <lvgl/AssetFile.hpp>

#include "StringPrinter.hpp"

#include "designlab/fonts/FontAwesomeIcons.hpp"

#include "FontManager.hpp"

FontManager::FontManager(const Construct &options) : m_construct(options) {

  m_icons = options.icons;

  m_lv_font_conv_path = options.lv_font_conv_path.is_empty()
                          ? Process::which("lv_font_conv")
                          : options.lv_font_conv_path;

  const auto node_path = options.node_path;

  if (m_lv_font_conv_path.is_empty()) {
    API_RETURN_ASSIGN_ERROR("`lv_font_conv` not found on the path", EINVAL);
  }

  const auto input_object
    = load_json_file(options.project_path / options.input_path);
  API_RETURN_IF_ERROR();

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

  m_progress_total = [&]() {
    size_t result = 0;
    for (const auto &font : font_list) {
      const auto sizes = get_user_size_list(font);
      result += sizes.string_view().split(",").count();
    }
    if (options.theme_font_list) {
      result += options.theme_font_list->count();
    }
    return result;
  }();

  for (const auto &font : font_list) {
    const auto sizes = get_user_size_list(font);
    for (const auto font_size : sizes.string_view().split(",")) {
      process_font_size(
        output_directory,
        node_path,
        options,
        font,
        font_size,
        settings.is_fonts_compressed());
    }
  }

  if (options.theme_font_list) {
    for (const auto &theme_font : *(options.theme_font_list)) {
      const lvgl::Font::Info info{theme_font};
      for (const auto &font : font_list) {
        if (
          info.name() == font.get_name()
          && font.get_style() == lvgl::Font::to_cstring(info.style())) {
          process_font_size(
            output_directory,
            node_path,
            options,
            font,
            var::NumberString(info.point_size()),
            settings.is_fonts_compressed());
        }
      }
    }

    {
      m_generated_container.sort(GeneratedContainer::ascending);
      GeneratedContainer tmp_container;
      for (const auto &generated : m_generated_container) {
        const auto offset = tmp_container.find_offset(generated);
        if (offset == tmp_container.count()) {
          tmp_container.push(generated);
        }
      }
      m_generated_container = std::move(tmp_container);
    }
  }

  const auto full_output_directory = options.project_path / output_directory;
#if USE_BOOTSTRAP_ICONS
  generate_bootstrap_icons_hpp(full_output_directory);
#endif
  generate_fontawesome_icons_hpp(full_output_directory);
  generate_fonts_source(full_output_directory, font_list);
}

void FontManager::process_font_size(
  var::StringView output_directory,
  var::StringView node_path,
  const Construct &options,
  const Settings::Font &font,
  var::StringView font_size,
  bool is_compressed) {
  var::GeneralString command = "lv_font_conv";

  const auto output_file_path
    = output_directory / get_file_name(font, font_size);

  StringPrinter printer;

  m_generated_container.push(get_file_name(font, font_size));

  if (m_construct.is_dry_run) {
    return;
  }

  Process::Arguments arguments(node_path);
  arguments.push(m_lv_font_conv_path)
    .push("--bpp=" | font.get_bits_per_pixel())
    .push("--size=" | font_size)
    .push("--format=lvgl")
    .push("--output=" | options.project_path / output_file_path.string_view())
    .push("--font=" | options.project_path / font.get_path())
    .push("--range=" | font.get_range());

  if (!is_compressed) {
    arguments.push("--no-compress");
  }

  if (font.is_icons()) {
    auto add_icon_range
      = [&](var::StringView font_name, var::StringView family) {
          const auto icon_range = get_icon_font_range(options.icons, family);
          if (icon_range.length()) {
            arguments.push("--font=" | get_temporary_font_path(font_name))
              .push("--range=" | icon_range);
          }
        };

    add_icon_range(font_solid_name, "solid");
    add_icon_range(font_regular_name, "regular");
    add_icon_range(font_brands_name, "brands");
  }

  auto env = Process::Environment().set_working_directory(options.project_path);

  if (!node_path.is_empty()) {
    const auto current_path = getenv("PATH");
    if (current_path != nullptr && 0) {
      env.set("PATH", var::StringView(current_path) | ":" | node_path);
    } else {
      env.set("PATH", node_path);
    }
  }

  printer.object("arguments", arguments).object("environment", env);

  Process lv_font_conv(arguments, env);

  lv_font_conv.wait();
  lv_font_conv.read_standard_output();

  printer.key("output", lv_font_conv.get_standard_output())
    .key("error", lv_font_conv.get_standard_error());

  auto status = lv_font_conv.status();
  auto exit_status = status.exit_status();
  printer.key("exitStatus", var::NumberString(exit_status));

  const auto log_file_path
    = SessionSettings::get_application_directory() / "lv_font_conv_log.txt";
  File(File::IsOverwrite::yes, log_file_path).write(printer.output());

  if (exit_status != 0) {

    API_RETURN_ASSIGN_ERROR(
      "`lv_font_conv` had an error (see `" | log_file_path.string_view()
        | "` for details)",
      EINVAL);
  }

  m_progress_value++;

  if (options.update_callback) {
    options.update_callback(
      options.update_context,
      m_progress_value,
      m_progress_total);
  }
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
    const auto sizes = get_user_size_list(font);
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
    code_printer.inline_comment("This is a generated source file, do not edit")
      .newline();
    cprinter::CppPrinter::HeaderGuard header_guard(
      code_printer,
      "BOOTSTRAP_ICONS_HPP");
    cprinter::CppPrinter::NamespaceScope icons_namespace(code_printer, "icons");
    cprinter::CppPrinter::NamespaceScope fa_namespace(code_printer, "bs");
    const auto icons = m_icons;
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
    code_printer.inline_comment("This is a generated source file, do not edit")
      .newline();
    cprinter::CppPrinter::HeaderGuard header_guard(
      code_printer,
      "FONTAWESOME_ICONS_HPP");
    {
      cprinter::CppPrinter::NamespaceScope icons_namespace(
        code_printer,
        "icons");
      cprinter::CppPrinter::NamespaceScope fa_namespace(code_printer, "fa");
      const auto icons = m_icons;
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

    code_printer.newline();

    for (const auto &icon : m_icons.get_range()) {
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
                String::Replace().set_old_string("-").set_new_string("_"))
              .to_upper();

        code_printer.line(
          "#define ICONS_FA_" | name.string_view() | " \"\\"
          | icon.get_unicode() | "\"");
      }
    }
  }

  code_printer.newline();
  code_printer.newline();
}

void FontManager::generate_fonts_source(
  var::StringView directory,
  const var::Vector<Settings::Font> &font_list) {

  File fonts_h(File::IsOverwrite::yes, directory / "fonts.h");
  File fonts_c(File::IsOverwrite::yes, directory / "fonts.c");

  CodePrinter h_printer(fonts_h);
  CodePrinter c_printer(fonts_c);

  const auto count = [&]() {
    int result = 0;
    for_each_user_font<int>(
      font_list,
      result,
      [](const Settings::Font &, var::StringView, int &count) { ++count; });
    return result;
  }();

  const GeneralString lvgl_font_list
    = "static const lvgl_api_font_descriptor_t fonts_list["
      | NumberString(count) | "]";

  {
    CodePrinter::HeaderGuard header_guard(h_printer, "DESIGNLAB_FONTS_H_");
    h_printer.newline();
    h_printer.line("#include <lvgl_api.h>")
      .newline()
      .line("#if defined __cplusplus")
      .line("extern \"C\" {")
      .line("#endif")
      .newline()
      .line("const lvgl_api_font_descriptor_t *fonts_get_font(int offset);")
      .line("#if defined __link")
      .line("void fonts_initialize();")
      .line("#else")
      .statement("extern " | lvgl_font_list)
      .line("#endif")
      .newline()
      .line("#if defined __cplusplus")
      .line("}")
      .line("#endif")
      .newline();
  }

  c_printer.newline().newline();

  c_printer.line("#include \"fonts.h\"").newline().newline();

  static auto get_abbreviated_string = [](var::StringView style) {
    return lvgl::Font::to_abbreviated_cstring(
      lvgl::Font::style_from_string(style));
  };

  for_each_user_font<CodePrinter>(
    font_list,
    c_printer,
    [](
      const Settings::Font &font,
      var::StringView font_size,
      CodePrinter &c_printer) {
      auto name = font.get_name() & "_"
                  & get_abbreviated_string(font.get_style()) & "_" & font_size;
      c_printer.statement("extern const lv_font_t " | name);
    });

  c_printer.newline().newline();

  {
    CodePrinter::StructInitialization struct_init(c_printer, lvgl_font_list);

    for_each_user_font<CodePrinter::StructInitialization>(
      font_list,
      struct_init,
      [](
        const Settings::Font &font,
        var::StringView font_size,
        CodePrinter::StructInitialization &struct_init) {
        const auto name = font.get_name() & "_"
                          & get_abbreviated_string(font.get_style()) & "_"
                          & font_size;
        const auto text_name = PathString(name).replace(
          PathString::Replace().set_old_character('_').set_new_character('-'));

        struct_init.add_member(
          "{ .name = \"" | text_name | "\", .font = &" | name | "}");
      });
  }

  c_printer.newline().newline();
  {
    CodePrinter::Function get_font_function(
      c_printer,
      "const lvgl_api_font_descriptor_t *fonts_get_font(int offset)");

    c_printer.statement(
      "const int count = sizeof(fonts_list) / "
      "sizeof(lvgl_api_font_descriptor_t)");

    {
      CodePrinter::IfScope check_offset(
        c_printer,
        "offset >= 0 && offset < count");
      c_printer.statement("return fonts_list + offset");
    }
    c_printer.statement("return NULL");
  }

  {
    CodePrinter::Function get_font_function(
      c_printer,
      "void fonts_initialize()");
    c_printer.newline().line("#if defined __link");
    c_printer.statement("lvgl_api_set_font_callback(fonts_get_font)");
    c_printer.line("#endif").newline();
  }
}
