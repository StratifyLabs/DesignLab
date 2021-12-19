//
// Created by Tyler Gilbert on 12/12/21.
//

#include <fs.hpp>
#include <var.hpp>
#include <json.hpp>


#include "FontAwesomeManager.hpp"

FontAwesomeManager::FontAwesomeManager(const sys::Cli &cli) {
  const auto fa_json = load_json_file(cli.get_option("input"));

  JsonObject fa_solid;
  JsonObject fa_regular;
  JsonObject fa_brands;

  const auto key_list = fa_json.get_key_list();
  for(const auto & key: key_list){
    const auto key_object = fa_json.at(key).to_object();

    const auto free_array = key_object.at("free").to_array();
    if( free_array.is_valid() ){
      for(const auto & kind: free_array){
        if( kind.to_string_view() == "solid" ){
          fa_solid.insert(key, key_object.at("unicode"));
        }
        if( kind.to_string_view() == "regular" ){
          fa_regular.insert(key, key_object.at("unicode"));
        }
        if( kind.to_string_view() == "brands" ){
          fa_brands.insert(key, key_object.at("unicode"));
        }
      }
    }
  }

  const auto output_path = cli.get_option("output");
  JsonDocument().save(fa_solid, File(File::IsOverwrite::yes, output_path / "fa-solid.json"));
  JsonDocument().save(fa_regular, File(File::IsOverwrite::yes, output_path / "fa-regular.json"));
  JsonDocument().save(fa_brands, File(File::IsOverwrite::yes, output_path / "fa-brands.json"));

}

