//
// Created by Tyler Gilbert on 12/12/21.
//

#ifndef DESIGNLAB_LOGIC_BOOTSTRAPICONMANAGER_HPP
#define DESIGNLAB_LOGIC_BOOTSTRAPICONMANAGER_HPP

#include <sys/Cli.hpp>

#include "Manager.hpp"

class BootstrapIconManager : public Manager {
public:
  explicit BootstrapIconManager(const sys::Cli &cli);

  struct Construct {
    API_PMAZ(input_path,Construct,var::PathString,{});
    API_PMAZ(output_path,Construct,var::PathString,{});
  };

  explicit BootstrapIconManager(const Construct & options){
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

#endif // DESIGNLAB_LOGIC_BOOTSTRAPICONMANAGER_HPP
