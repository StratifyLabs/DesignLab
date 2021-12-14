//
// Created by Tyler Gilbert on 12/12/21.
//

#ifndef DESIGNLAB_FONTMAKER_HPP
#define DESIGNLAB_FONTMAKER_HPP

#include <design/extras/Form.hpp>

#include "ScreenObject.hpp"

class FontMaker : public ScreenObject {
public:
  static void configure(Generic generic);

private:
  static void configure_form_input(Form form);

};

#endif // DESIGNLAB_FONTMAKER_HPP
