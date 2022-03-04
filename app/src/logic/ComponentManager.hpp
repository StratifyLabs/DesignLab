//
// Created by Tyler Gilbert on 3/4/22.
//

#ifndef DESIGNLAB_COMPONENTMANAGER_HPP
#define DESIGNLAB_COMPONENTMANAGER_HPP

#include <fs/DataFile.hpp>

#include "view/extras/AddComponent.hpp"
#include "model/Settings.hpp"

#include "Manager.hpp"

class ComponentManager : public Manager {
public:

  using Fields = AddComponent::Fields;

  ComponentManager & set_component(const json::JsonObject object){
    m_component = object;
    return *this;
  }

  ComponentManager & generate();

  const var::String header_file_contents() const {
    return m_header_contents;
  }

  const var::String source_file_contents() const {
    return m_source_contents;
  }


private:
  Settings::Component m_component;

  var::String m_header_contents;
  var::String m_source_contents;

  fs::DataFile m_header_file;
  fs::DataFile m_source_file;

  CodePrinter m_header_printer = CodePrinter(m_header_file);
  CodePrinter m_source_printer = CodePrinter(m_source_file);


  void generate_header();
  void generate_source();

  void process_header_node(json::JsonObject node);
  void add_to_names_struct(json::JsonObject node);
  void process_source_node(json::JsonObject node){}

};

#endif // DESIGNLAB_COMPONENTMANAGER_HPP
