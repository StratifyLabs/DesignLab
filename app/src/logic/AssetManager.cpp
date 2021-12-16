//
// Created by Tyler Gilbert on 12/15/21.
//

#include <fs.hpp>
#include <json.hpp>
#include <var.hpp>

#include "AssetManager.hpp"
#include "model/Model.hpp"

AssetManager::AssetManager(const sys::Cli &cli) {
  construct(
    {.input_path = cli.get_option("input"),
     .output_path = cli.get_option("output")});
}

void AssetManager::construct(const AssetManager::Construct &options) {
  const auto input_object = load_json_file(options.input_path);
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
      .size = info.size(),
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

  // convert the data to a .c file
  auto output_file
    = File(File::IsOverwrite::yes, output_directory / "assets.c");

  m_code_printer = CodePrinter(output_file);

  {
    CodePrinter::StructInitialization asset_data(
      m_code_printer,
      "const char assets[]");
    const auto &data = data_file.data();
    static constexpr size_t line_size = 64;
    for (size_t offset = 0; offset < data.size(); offset += line_size) {
      GeneralString line;
      const auto remaining = data.size() - offset;
      const auto page_size = remaining > line_size ? line_size : remaining;
      for (const auto i : api::Index(page_size)) {
        line |= NumberString(data.data_u8()[offset + i], "%02x,");
      }
      if( offset + page_size >= data.size() ){
        line.pop_back();
      }
      m_code_printer.line(line);
    }
  }

}


