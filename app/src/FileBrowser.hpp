#ifndef FILEBROWSER_HPP
#define FILEBROWSER_HPP

#include "Application.hpp"

class FileBrowser : public Application {
public:

  static void configure(lvgl::Container & container);

private:

  static void configure_directory(lvgl::Container & container);
};

#endif // FILEBROWSER_HPP
