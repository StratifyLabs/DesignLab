//
// Created by Tyler Gilbert on 12/2/21.
//

#ifndef DESIGNLAB_VIEWOBJECT_HPP
#define DESIGNLAB_VIEWOBJECT_HPP

#include "printer/YamlPrinter.hpp"
#include "thread/Mutex.hpp"
#include "thread/Thread.hpp"

using namespace printer;

#include <design.hpp>
#include <lvgl.hpp>

#include <design/extras/Form.hpp>
#include <design/extras/FormList.hpp>

#include "designlab/fonts/Icons.hpp"
#include "model/Model.hpp"

class ViewObject : public ModelAccess {
public:

protected:
  struct Names {
    static constexpr auto content_container = "ContentContainer";
  };

};

#endif // DESIGNLAB_VIEWOBJECT_HPP
