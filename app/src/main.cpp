// COPYING: Copyright 2017-2021 Tyler Gilbert and Stratify Labs. All rights
// reserved

// This must be included to handle
// SDL's main changing scheme
#include <SDL_main.h>

#include <sys/Cli.hpp>

#include "Application.hpp"

#include "logic/ThemeManager.hpp"
#include "logic/BootstrapIconManager.hpp"
#include "logic/FontAwesomeManager.hpp"

#include <cprinter.hpp>

int main(int argc, char *argv[]) {
  sys::Cli cli(argc, argv);


  if( !cli.get_option("fa").is_empty() ){
    FontAwesomeManager manager(cli);
    if (api::ExecutionContext::is_error()) {
      manager.printer().object("error", api::ExecutionContext::error());
    }
    exit(api::ExecutionContext::is_error() ? 1 : 0);
  }

  if( !cli.get_option("theme").is_empty() ){
    ThemeManager manager(cli);
    if (api::ExecutionContext::is_error()) {
      manager.printer().object("error", api::ExecutionContext::error());
    }
    exit(api::ExecutionContext::is_error() ? 1 : 0);
  }

  if( !cli.get_option("icons").is_empty() ){
    BootstrapIconManager manager(cli);
    if (api::ExecutionContext::is_error()) {
      manager.printer().object("error", api::ExecutionContext::error());
    }
    exit(api::ExecutionContext::is_error() ? 1 : 0);
  }


  Application::run(cli);

  return 0;
}
