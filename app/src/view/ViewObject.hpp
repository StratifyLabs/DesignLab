//
// Created by Tyler Gilbert on 12/2/21.
//

#ifndef DESIGNLAB_VIEWOBJECT_HPP
#define DESIGNLAB_VIEWOBJECT_HPP

#include <design/macros.hpp>

#include <printer/YamlPrinter.hpp>
#include <thread/Mutex.hpp>
#include <thread/Thread.hpp>

#include <design/extras/Prompt.hpp>

using namespace printer;

#include <design.hpp>
#include <lvgl.hpp>

#include <design/extras/Form.hpp>
#include <design/extras/FormList.hpp>

#include "designlab/fonts/FontAwesomeIcons.hpp"
#include "model/Model.hpp"

class ViewObject : public ModelAccess {
public:
  static void send_notify_to_home() {
    Event::send(
      screen().find<Generic>(Names::home_top_row),
      EventCode::notified);
  }

  class NotifyHome {
  public:
    NotifyHome() = default;
    ~NotifyHome() { send_notify_to_home(); }
  };

  static void prompt_user(design::Prompt::Data &data) {
    Modal modal(Names::prompt_modal);
    modal.add_content(
      Prompt(data).set_width(60_percent).set_height(40_percent));
  }

  static void close_prompt(lv_event_t *e) {
    Event(e).find_parent<Modal>(Names::prompt_modal).close(300_milliseconds);
  }

protected:
  struct Names {
    DESIGN_DECLARE_NAME(home_top_row);
    DESIGN_DECLARE_NAME(home_container);
    DESIGN_DECLARE_NAME(content_container);
    DESIGN_DECLARE_NAME(prompt_modal);

    DESIGN_DECLARE_NAME(home_object);
    DESIGN_DECLARE_NAME(about_object);
    DESIGN_DECLARE_NAME(builder_object);
    DESIGN_DECLARE_NAME(project_object);
    DESIGN_DECLARE_NAME(asset_maker_object);
    DESIGN_DECLARE_NAME(color_preview_object);
    DESIGN_DECLARE_NAME(font_maker_object);
    DESIGN_DECLARE_NAME(icon_maker_object);
    DESIGN_DECLARE_NAME(theme_maker_object);

  };

};

#endif // DESIGNLAB_VIEWOBJECT_HPP
