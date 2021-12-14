//
// Created by Tyler Gilbert on 12/13/21.
//

#include "AssetMaker.hpp"

void AssetMaker::configure(Generic generic) {
  generic.add(Container().fill().add(Column().fill().setup([](Column column) {
    column.add(ScreenHeading("Assets")).add(HorizontalLine());

    column.add(SubSectionHeading("Add files to include in the binary"));
  })));
}
