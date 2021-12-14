#ifndef TOOLS_HPP
#define TOOLS_HPP

#include <lvgl/List.hpp>

#include "Elements.hpp"

class Tools : public Application {
public:
  static void configure(Generic container) {

    static constexpr auto add_button_name = "AddButton";
    static constexpr auto add_list_name = "AddList";

    static const auto checklist_context = CheckList::Data("");
    static const auto checklist_context_allow_multiple
      = CheckList::Data("").set_allow_multiple();
    static auto formlist_context = FormList::Data("");
    API_PRINTF_TRACE_LINE();

    container.add(Label("Tools"))
      .add(Button(add_button_name)
             .add_label("Button")
             .add_event_callback(
               EventCode::clicked,
               [](lv_event_t *) {
                 fflush(stdout);
                 screen()
                   .find<Generic>(canvas_container_name)
                   .add(Button(generate_name()));

                 auto button = screen().find<Button>(latest_name());
                 button.set_width(100).set_height(200);

                 notify_properties(button.object());
               }))
      .add(Button("").add_label("Label").add_event_callback(
        EventCode::clicked,
        [](lv_event_t *) {
          fflush(stdout);
          Generic(model().selected_object).add(Label(generate_name()));

          auto button = screen().find<Button>(latest_name());
          button.set_width(100).set_height(200);

          notify_properties(button.object());
        }))
      .add(Button(add_list_name).add_label("List"))
#if 1
      .add(CheckList(checklist_context).setup([](CheckList check_list) {
        check_list.set_width(100_percent)
          .add_item("Banana", "Banana")
          .add_item("Apple", "Apple")
          .add_item("grapes", "grapes")
          .add_item("nectar", "nectar")
          .add_item("lemon", "lemon")
          .add_item("lime", "lime")
          .add_item("Orange", "Orange");
      }))
      .add(CheckList(checklist_context_allow_multiple)
             .setup([](CheckList check_list) {
               check_list.set_width(100_percent)
                 .add_item("Banana", "Banana")
                 .add_item("Apple", "Apple")
                 .add_item("grapes", "grapes")
                 .add_item("nectar", "nectar")
                 .add_item("lemon", "lemon")
                 .add_item("lime", "lime")
                 .add_item("Orange", "Orange");
             }))
      .add(FormList(formlist_context).setup([](FormList form_list) {
        using ItemData = FormList::ItemData;

        form_list.set_width(100_percent)
          .add_item(FormList::ItemData::create("Banana"))
          .add_item(FormList::ItemData::create("Path")
                      .set_type(FormList::ItemType::string)
                      .set_clicked_callback([](lv_event_t *) {
                        // open the keyboard -- and set the input
                        // callback to assign the value using c->set_value()
                        // c->set_value("/Users/tgil");
                      }));
      }))
#endif
      ;
  }
};

#endif // TOOLS_HPP
