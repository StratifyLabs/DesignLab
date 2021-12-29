//
// Created by Tyler Gilbert on 12/29/21.
//

#ifndef DESIGNLAB_ABOUT_HPP
#define DESIGNLAB_ABOUT_HPP

#include "ViewObject.hpp"

class About : public ViewObject {
public:
  static void configure(Generic generic);

private:
  struct Names {
    DESIGN_DECLARE_NAME(about_column);
  };
};

#endif // DESIGNLAB_ABOUT_HPP
