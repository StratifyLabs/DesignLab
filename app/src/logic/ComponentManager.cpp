//
// Created by Tyler Gilbert on 3/4/22.
//

#include <design/extras/Utility.hpp>
#include <lvgl/Style.hpp>

#include "model/Model.hpp"

#include "ComponentManager.hpp"

using namespace fs;
using namespace var;
using namespace lvgl;
using namespace design;

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

void ComponentManager::generate_source() {
  API_RETURN_IF_ERROR();
  m_source_contents = "";

  const auto header_name = m_component.get_name() & ".hpp";
  const auto class_name = m_component.get_name();

  m_source_printer.newline();

  m_source_printer.header("lvgl.hpp", CodePrinter::IsQuotes::no);
  m_source_printer.header("design.hpp", CodePrinter::IsQuotes::no);

  // any design extras to include?
  m_source_printer.newline();
  m_source_printer.header(header_name);
  m_source_printer.newline();

  {
    CodePrinter::Function constructor(
      m_source_printer,
      class_name | "::" | class_name | "(const char * name)");

    m_source_printer.statement("construct_object(name)");

    // set top level settings

    // add sub objects
    for (const auto leaf : m_component.tree()) {
      process_source_node(leaf, IsNested::no);
    }
  }

  m_source_contents = m_source_file.data().add_null_terminator();
}

void ComponentManager::process_source_node(
  json::JsonObject node,
  IsNested is_nested) {
  Model::Scope ms;
  ModelAccess::printer().object("node", node);

  const auto name_value = node.at(Fields::component_name);
  if (!name_value.is_valid()) {
    printf("No name\n");
    return;
  }

  const auto class_value = node.at(Fields::component_type);

  if (!class_value.is_valid()) {
    printf("No type\n");
    return;
  }

  const auto class_name = class_value.to_string_view();

  const auto prefix = is_nested == IsNested::yes ? "." : "";

  m_source_printer.line_with_indent(
    prefix | StringView("add(") | class_value.to_string_view() | "(Names::"
    | name_value.to_string_view() | ")");

  add_modifiers(node);

  for (const auto leaf : node) {
    if (leaf.is_object()) {
      process_source_node(leaf.to_object(), IsNested::yes);
    }
  }

  m_source_printer.line_with_indent(")");
}

void ComponentManager::add_modifiers(json::JsonObject node) {

  const auto class_name = node.at(Fields::component_type).to_string_view();

  for (const auto description : Utility::property_list) {
    const auto property_name = Utility::to_cstring(description.property);
    const auto property_value = node.at(property_name);
    if (property_value.is_valid()) {
      const StringView property_type_class_name
        = Utility::to_class_name_cstring(description.type);
      if (
        property_type_class_name != Utility::invalid_property_type_class_name) {
        m_source_printer.line(
          StringView(".set_") | property_name | "(" | property_type_class_name
          | "::" | property_value.to_string_view());
      } else {
        m_source_printer.line(
          StringView(".set_") | property_name | "("
          | property_value.to_string_view() | ")");
      }
    }
  }

  if (class_name == AddComponent::Components::form_line_field) {
    m_source_printer.line(
      ".set_label_as_static(\""
      | node.at(Fields::component_form_line_field_label).to_string_view()
      | "\")");

    m_source_printer.line(
      ".set_hint_as_static(\""
      | node.at(Fields::component_form_line_field_hint).to_string_view()
      | "\")");
    return;
  }

  if (class_name == AddComponent::Components::form_switch) {
    m_source_printer.line(
      ".set_label_as_static(\""
      | node.at(Fields::component_form_switch_label).to_string_view() | "\")");

    m_source_printer.line(
      ".set_hint_as_static(\""
      | node.at(Fields::component_form_switch_hint).to_string_view() | "\")");
    return;
  }

  if (class_name == AddComponent::Components::form_select) {
    m_source_printer.line(
      ".set_label_as_static(\""
      | node.at(Fields::component_form_select_label).to_string_view() | "\")");

    m_source_printer.line(
      ".set_hint_as_static(\""
      | node.at(Fields::component_form_select_hint).to_string_view() | "\")");
    return;
  }

  if (class_name == AddComponent::Components::form_file_select) {
    m_source_printer.line(
      ".set_label_as_static(\""
      | node.at(Fields::component_form_file_select_label).to_string_view()
      | "\")");

    m_source_printer.line(
      ".set_hint_as_static(\""
      | node.at(Fields::component_form_file_select_hint).to_string_view()
      | "\")");
    return;
  }

  if (class_name == AddComponent::Components::button) {
    const auto label_value = node.at(Fields::component_button_label);
    if (label_value.is_valid()) {
      m_source_printer.line(
        ".add_label_as_static(\"" | label_value.to_string_view() | "\")");
    }
  }
}