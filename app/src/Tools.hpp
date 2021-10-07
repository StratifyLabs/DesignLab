#ifndef TOOLS_HPP
#define TOOLS_HPP

#include "Application.hpp"

class Tools : public Application {
public:
  static void configure(Container &container) {

    static constexpr auto add_button_name = "AddButton";
    static constexpr auto add_list_name = "AddList";

    static auto screen = Container::active_screen();

    container.add(E::Title("", "Tools"))
      .add(E::Spacer())
      .add(E::AddButton(add_button_name)
             .set_initial_text_static("Button")
             .set_initial_clicked([](lv_event_t *) {
               printf("add button\n");
               fflush(stdout);
               screen.find(canvas_container_name)
                 .cast<Container>()
                 ->add(Button(generate_name()));

               auto *button = screen.find(latest_name()).cast<Button>();
               button->set_width(100).set_height(200);


               notify_properties(button->object());
             }))
      .add(E::Spacer())
      .add(E::AddButton(add_list_name).set_initial_text_static("List"));
  }
};

#endif // TOOLS_HPP
