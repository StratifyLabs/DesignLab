// COPYING: Copyright 2017-2021 Tyler Gilbert and Stratify Labs. All rights
// reserved

#include <QApplication>
#include <QFile>
#include <QScreen>
#include <QVariant>

#include <lvgl.hpp>

#include "fonts/fonts.h"

// Local
#include "LvglRenderer.hpp"
#include "MainWindow.hpp"

#include "Application.hpp"

#include <fs/ViewFile.hpp>

int main(int argc, char *argv[]) {

  QApplication app(argc, argv);

  MainWindow window;

  fonts_initialize();

  static constexpr auto width = LvglRenderer::max_width / 2;
  static constexpr auto height = LvglRenderer::max_height / 2;

  window.setMinimumWidth(width);
  // window.setWidth(width);
  window.setMaximumWidth(width);
  window.setMinimumHeight(height);
  // window.setHeight(height);
  window.setMaximumHeight(height);

#if 1
  auto availableGeometry = app.primaryScreen()->availableGeometry();
  window.setGeometry(
    availableGeometry.width() / 2 - width / 2,
    availableGeometry.height() / 2 - height / 2,
    width,
    height);
#endif

  lvgl_api_initialize_filesystem();

#if _LVGL_HAS_STRATIFY_OS
  lv_fs_drv_t asset_drv;
  auto assetfs = window.get_assets(":/assets.fs.assetfs");
  lvgl_api_mount_asset_filesystem(assetfs.data(), &asset_drv, 'a');
#endif

  Application application(window.keyboard_group());

  window.show();
  return app.exec();
}
