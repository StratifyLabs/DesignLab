// COPYING: Copyright 2017-2022 Tyler Gilbert and Stratify Labs. All rights
// reserved

// This must be included to handle
// SDL's main changing scheme
#include <SDL_main.h>

#include <sys/Cli.hpp>
#include <api/api.hpp>

#include "Application.hpp"

int main(int argc, char *argv[]) {
  api::catch_segmentation_fault();
  sys::Cli cli(argc, argv);
  Application::run(cli);
  return 0;
}
