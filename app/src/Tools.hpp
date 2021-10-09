#ifndef TOOLS_HPP
#define TOOLS_HPP

#include <lvgl/List.hpp>

#include "Elements.hpp"

class Tools : public Application {
public:
  static void configure(Container &container) {

    static constexpr auto add_button_name = "AddButton";
    static constexpr auto add_list_name = "AddList";

    static auto screen = Container::active_screen();

    static const auto checklist_context = CheckList::UserData("");
    static const auto checklist_context_allow_multiple
      = CheckList::UserData("").set_allow_multiple();
    static const auto formlist_context = FormList::UserData("");
    API_PRINTF_TRACE_LINE();

    container.add(E::Title("", "Tools"))
      .add(E::Spacer())
      .add(E::AddButton(add_button_name)
             .set_initial_text_static("Button")
             .set_initial_clicked([](lv_event_t *) {
               fflush(stdout);
               screen.find(canvas_container_name)
                 .cast<Container>()
                 ->add(Button(generate_name()));

               auto *button = screen.find(latest_name()).cast<Button>();
               button->set_width(100).set_height(200);

               notify_properties(button->object());
             }))
      .add(E::Spacer())
      .add(
        E::AddButton("").set_initial_text_static("Label").set_initial_clicked(
          [](lv_event_t *) {
            fflush(stdout);
            Container(model().selected_object).add(Label(generate_name()));

            auto *button = screen.find(latest_name()).cast<Button>();
            button->set_width(100).set_height(200);

            notify_properties(button->object());
          }))
      .add(E::Spacer())
      .add(E::AddButton(add_list_name).set_initial_text_static("List"))
#if 1
      .add(CheckList(checklist_context).configure([](CheckList &check_list) {
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
             .configure([](CheckList &check_list) {
               check_list.set_width(100_percent)
                 .add_item("Banana", "Banana")
                 .add_item("Apple", "Apple")
                 .add_item("grapes", "grapes")
                 .add_item("nectar", "nectar")
                 .add_item("lemon", "lemon")
                 .add_item("lime", "lime")
                 .add_item("Orange", "Orange");
             }))
      .add(FormList(formlist_context).configure([](FormList &form_list) {

        using ItemUserData = FormList::ItemUserData;

        form_list.set_width(100_percent)
          .add_item(ItemUserData::create("Banana"))
          .add_item(
            ItemUserData::create("Path")
              .set_type(FormList::ItemType::string)
              .set_edit_callback([](FormList::ItemUserData *c, lv_event_t *) {
                // open the keyboard -- and set the input
                // callback to assign the value using c->set_value()
                c->set_value("/Users/tgil");
              }));
      }))
#endif
      ;
  }
};

#endif // TOOLS_HPP
