//
// Created by Tyler Gilbert on 12/12/21.
//

#include <fs.hpp>
#include <var.hpp>

#include "BootstrapIconManager.hpp"

BootstrapIconManager::BootstrapIconManager(const sys::Cli &cli) {
  construct(
    {.input_path = cli.get_option("input"),
     .output_path = cli.get_option("output")});
}

void BootstrapIconManager::construct(const Construct &options) {

  {
    const auto object_path = options.input_path / "font/bootstrap-icons.json";
    const auto object = load_json_file(object_path);
    API_RETURN_IF_ERROR();
    const auto key_list = object.get_key_list();
    m_icon_list.reserve(key_list.count());
    for (const auto &key : key_list) {
      m_icon_list.push_back(
        {.name = key, .unicode = u32(object.at(key).to_integer())});
    }
    if (m_icon_list.count() == 0) {
      API_RETURN_ASSIGN_ERROR("no icons were found in " | object_path, EINVAL);
    }
  }

  {
    const auto output_path = options.output_path;
    if (!is_create_file_ok(output_path)) {
      return;
    }

    File output_file(File::IsOverwrite::yes, output_path);
    API_RETURN_IF_ERROR();
    m_code_printer = CodePrinter(output_file);

    {
      m_code_printer.inline_comment("This is a generated source file, do not edit").newline();
      cprinter::CppPrinter::HeaderGuard header_guard(m_code_printer, "BOOTSTRAP_ICONS_HPP");
      cprinter::CppPrinter::NamespaceScope icons_namespace(m_code_printer, "icons");
      cprinter::CppPrinter::NamespaceScope bs_namespace(m_code_printer, "bs");
      for (const auto &icon : m_icon_list) {
        const auto needs_x_prefix = [&](){
          if( icon.name.length() ){
            const auto first = icon.name.at(0);
            return first >= '0' || first <= '9';
          }
          return false;
        }();

        const auto name
          = String((needs_x_prefix ? "x" : "") + icon.name.string_view())
              .replace(
                String::Replace().set_old_string("-").set_new_string("_"));



        m_code_printer.statement(
          "static constexpr auto " | name.string_view() | " = "
          | NumberString(icon.unicode, "\"\\u%04x\""));
      }
    }
  }
}
