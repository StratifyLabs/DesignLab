//
// Created by Tyler Gilbert on 12/13/21.
//

#ifndef DESIGNLAB_ASSETMAKER_HPP
#define DESIGNLAB_ASSETMAKER_HPP

#include "Editor.hpp"
#include "ViewObject.hpp"
#include "extras/Extras.hpp"

class AssetMaker : public ViewObject, public ObjectAccess<AssetMaker> {
public:
  static void configure(Generic generic){
    generic.add(AssetMaker(ViewObject::Names::asset_maker_object));
  }
private:
  LVGL_OBJECT_ACCESS_DECLARE_CONSTRUCTOR(AssetMaker);
};

#endif // DESIGNLAB_ASSETMAKER_HPP
