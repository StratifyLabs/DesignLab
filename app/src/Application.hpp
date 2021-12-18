#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <sys/Cli.hpp>

#include "view/ViewObject.hpp"

using namespace lvgl;

class Application : public ViewObject {

public:
  static void run(sys::Cli & cli);

protected:

private:
};

#endif // APPLICATION_HPP
