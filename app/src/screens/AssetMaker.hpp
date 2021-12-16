//
// Created by Tyler Gilbert on 12/13/21.
//

#ifndef DESIGNLAB_ASSETMAKER_HPP
#define DESIGNLAB_ASSETMAKER_HPP

#include "Maker.hpp"

class AssetMaker : public Maker {
public:
  static void configure(Generic generic);

private:

  class InputSchema : var::Vector<json::JsonObject> {
  public:
    InputSchema();

    static Form::Schema get_form_schema() {
      Form::Schema result;
      result.set_name(Settings::assets_key())
        .set_type(Form::Schema::schema_type)
        .set_input(InputSchema());
      return result;
    }
  };

  static void handle_add(lv_event_t*);
  static void configure_form_input(Generic generic);
  static void show_assets(Column column);
};

#endif // DESIGNLAB_ASSETMAKER_HPP
