//
// Created by Tyler Gilbert on 12/15/21.
//

#include <fs.hpp>
#include <json.hpp>
#include <var.hpp>

#include "AssetManager.hpp"

AssetManager::AssetManager(const sys::Cli &cli) {
  construct(
    {.input_path = cli.get_option("input"),
     .output_path = cli.get_option("output")});
}

void AssetManager::construct(const AssetManager::Construct &options) {
  const auto input_object
    = load_json_file(options.project_path / options.input_path);
  API_RETURN_IF_ERROR();

  const auto settings = Settings(input_object);
  const auto asset_list = settings.get_assets();
  const auto output_directory
    = options.project_path / settings.get_output_directory() / "assets";

  /*
   * create a .c file that has the assetfs directory and content
   *
   */

  DataFile data_file;

  lvgl_assetfs_header_t header{.count = asset_list.count()};

  data_file.write(View(header.count));

  u32 offset = sizeof(u32) + sizeof(lvgl_assetfs_dirent_t) * asset_list.count();
  for (const auto &asset : asset_list) {
    const auto info
      = FileSystem().get_info(options.project_path / asset.get_path());

    lvgl_assetfs_dirent_t entry{
      .start = offset,
      .mode = 0444,
      .size = u32(info.size()),
      .uid = 0};

    const auto file_name = Path::name(asset.get_path());
    View(entry.name).fill<u8>(0).copy(file_name);
    data_file.write(View(entry));
    offset += info.size();
  }

  for (const auto &asset : asset_list) {
    const auto asset_file = File(options.project_path / asset.get_path());
    data_file.write(asset_file);
  }

  // save the binary output
  File(File::IsOverwrite::yes, output_directory / "assets.assetfs")
    .write(data_file.seek(0));

}

fs::PathList
AssetManager::get_source_list(const var::StringView, const Settings &settings) {
  return {};
}
