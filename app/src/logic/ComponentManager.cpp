//
// Created by Tyler Gilbert on 3/4/22.
//

#include "ComponentManager.hpp"

using namespace fs;
using namespace var;

ComponentManager &ComponentManager::generate() {

  generate_header();
  generate_source();

  return *this;
}

void ComponentManager::generate_header() {
  API_RETURN_IF_ERROR();
  m_header_contents = "";

  m_header_file = DataFile();

  const auto guard_name
    = "DESIGN_LAB_COMPONENTS_"
      & NameString(m_component.get_name()).to_upper().string_view() & "_HPP";
  const auto class_name = m_component.get_name();

  m_header_printer.newline();
  m_header_printer.line("#ifndef " | guard_name);
  m_header_printer.line("#define " | guard_name);
  m_header_printer.newline();
  m_header_printer.newline();
  m_header_printer.header("lvgl/ObjectAccess.hpp", CodePrinter::IsQuotes::no);
  m_header_printer.header("design/macros.hpp", CodePrinter::IsQuotes::no);
  // add design::Worker

  m_header_printer.newline();
  m_header_printer.newline();

  {
    CodePrinter::ClassScope class_scope(
      m_header_printer,
      class_name | " : public lvgl::ObjectAccess<" | class_name | ">");
    m_header_printer.line("public:");
    m_header_printer.statement(
      "LVGL_OBJECT_ACCESS_DECLARE_CONSTRUCTOR(" | class_name | ")");
    m_header_printer.newline();
    // add a Data Struct?
    if (m_component.is_generate_data()) {
      CodePrinter::StructScope data_struct_scope(
        m_header_printer,
        "Data : lvgl::UserDataAccess<Data>");
      m_header_printer.line_with_indent(
        "Data(const char * name = \"\") : lvgl::UserDataAccess<Data>(name){}");
    }

    m_header_printer.newline();

    // add a worker Struct?
    if (m_component.is_generate_worker()) {
      CodePrinter::ClassScope worker_class_scope(
        m_header_printer,
        "Worker : design::WorkerAccess<Worker>");
      m_header_printer.line("public:");
      m_header_printer.statement("Worker() = default");
      m_header_printer.line_with_indent(
        "Worker(lvgl::Runtime * runtime) : design::WorkerAccess(runtime){}");
      m_header_printer.newline();
      m_header_printer.line("private:");
      m_header_printer.statement("void work() override");
    }

    m_header_printer.newline();
    m_header_printer.line("private:");
    {
      CodePrinter::StructScope name_scope(m_header_printer, "Names");
      add_to_names_struct(m_component.tree());
    }
  }

  m_header_printer.statement("#endif //" | guard_name);
  m_header_printer.newline();

  m_header_contents = m_header_file.data().add_null_terminator();
}

void ComponentManager::generate_source() {
  API_RETURN_IF_ERROR();
  m_source_contents = "";
}

void ComponentManager::process_header_node(json::JsonObject node) {}

void ComponentManager::add_to_names_struct(json::JsonObject node) {
  const auto name_value = node.at(Fields::component_name);
  if (name_value.is_valid()) {
    const auto name = node.at(Fields::component_name).to_string_view();
    if (name.is_empty() == false) {
      m_header_printer.statement("DESIGN_DECLARE_NAME(" | name | ")");
    }
  }
  for (const auto leaf : node) {
    if (leaf.is_object()) {
      add_to_names_struct(leaf.to_object());
    }
  }
}
