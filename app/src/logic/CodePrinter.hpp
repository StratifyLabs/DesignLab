//
// Created by Tyler Gilbert on 12/12/21.
//

#ifndef DESIGNLAB_LOGIC_CODEPRINTER_HPP
#define DESIGNLAB_LOGIC_CODEPRINTER_HPP

#include <cprinter/CppPrinter.hpp>
#include <fs/File.hpp>

class CodePrinter : public cprinter::CppPrinter {
public:
  CodePrinter() = default;
  CodePrinter(const fs::FileObject &output_file)
    : m_output_file(&output_file) {}

  void interface_print_final(var::StringView value) override {
    API_ASSERT(m_output_file);
    m_output_file->write(value);
  }

private:
  const fs::FileObject *m_output_file = nullptr;
};

#endif // DESIGNLAB_CODEPRINTER_HPP
