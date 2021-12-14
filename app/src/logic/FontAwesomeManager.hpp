//
// Created by Tyler Gilbert on 12/12/21.
//

#ifndef DESIGNLAB_LOGIC_FONTAWESOME_MANAGER_HPP
#define DESIGNLAB_LOGIC_FONTAWESOME_MANAGER_HPP

#include <sys/Cli.hpp>

#include "Manager.hpp"

class FontAwesomeManager : public Manager {
public:
  explicit FontAwesomeManager(const sys::Cli &cli);

  struct Construct {
    API_PMAZ(input_path,Construct,var::PathString,{});
    API_PMAZ(output_path,Construct,var::PathString,{});
  };

  explicit FontAwesomeManager(const Construct & options){
    construct(options);
  }

private:
  CodePrinter m_code_printer;
  fs::File m_output_file;

  struct Icon {
    var::KeyString name;
    u32 unicode;
  };

  var::Vector<Icon> m_icon_list;
  var::PathString m_font_folder_path;

  void construct(const Construct & options);
};

#endif // DESIGNLAB_LOGIC_FONTAWESOME_MANAGER_HPP
