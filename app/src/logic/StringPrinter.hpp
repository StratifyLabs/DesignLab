//
// Created by Tyler Gilbert on 12/12/21.
//

#ifndef DESIGNLAB_LOGIC_STRINGPRINTER_HPP
#define DESIGNLAB_LOGIC_STRINGPRINTER_HPP

#include <printer/Printer.hpp>

class StringPrinter : public printer::Printer {
public:
  void interface_print_final(var::StringView value) override {
    m_output += value;
  }

private:
  var::String m_output;
};

#endif // DESIGNLAB_STRINGPRINTER_HPP
