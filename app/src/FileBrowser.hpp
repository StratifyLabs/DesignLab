#ifndef FILEBROWSER_HPP
#define FILEBROWSER_HPP

#include "Application.hpp"

class FileBrowser : public Application {
public:

  static void configure(lvgl::Generic container);

private:

  static void configure_directory(lvgl::Generic container);
};

#endif // FILEBROWSER_HPP
