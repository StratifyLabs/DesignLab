//
// Created by Tyler Gilbert on 12/13/21.
//

#ifndef DESIGNLAB_ASSETMAKER_HPP
#define DESIGNLAB_ASSETMAKER_HPP

#include "ViewObject.hpp"
#include "extras/Extras.hpp"
#include "Editor.hpp"

class AssetMaker : public ViewObject {
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

  static var::Vector<InfoCard::Data::Feature> get_feature_list(json::JsonObject);
  static var::StringView get_info_title(json::JsonObject object);
  static Editor::IsValid validate(Form form);


};

#endif // DESIGNLAB_ASSETMAKER_HPP
