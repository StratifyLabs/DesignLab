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


private:
  CodePrinter m_code_printer;
  fs::File m_output_file;

  struct Icon {
    var::KeyString name;
    u32 unicode;
  };

  var::Vector<Icon> m_icon_list;
  var::PathString m_font_folder_path;

};

#endif // DESIGNLAB_LOGIC_FONTAWESOME_MANAGER_HPP
