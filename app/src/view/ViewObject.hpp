//
// Created by Tyler Gilbert on 12/2/21.
//

#ifndef DESIGNLAB_VIEWOBJECT_HPP
#define DESIGNLAB_VIEWOBJECT_HPP

#include <design/macros.h>

#include <printer/YamlPrinter.hpp>
#include <thread/Mutex.hpp>
#include <thread/Thread.hpp>

using namespace printer;

#include <design.hpp>
#include <lvgl.hpp>

#include <design/extras/Form.hpp>
#include <design/extras/FormList.hpp>

#include "designlab/fonts/FontAwesomeIcons.hpp"
#include "model/Model.hpp"

class ViewObject : public ModelAccess {
public:

  static void send_notify_to_home(){
    Event::send(screen().find<Generic>(Names::home_top_row), EventCode::notified);
  }

  class NotifyHome {
  public:
    NotifyHome() =default;
    ~NotifyHome(){
      send_notify_to_home();
    }
  };

protected:
  struct Names {
    DESIGN_DECLARE_NAME(home_top_row);
    DESIGN_DECLARE_NAME(home_container);
    DESIGN_DECLARE_NAME(content_container);
  };

};

#endif // DESIGNLAB_VIEWOBJECT_HPP
