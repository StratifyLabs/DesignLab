#ifndef ELEMENTS_HPP
#define ELEMENTS_HPP

#include "Application.hpp"

#include "Icons.hpp"

namespace lvgl {

OBJECT_ACCESS_FORWARD_FRIENDS();

class Elements {
public:
  static Group keyboard_group;

  class AddButton : public lvgl::ObjectAccess<AddButton> {
  public:
    explicit AddButton(const char *name = "") : ObjectAccess(name) {}

    static constexpr auto label_name = "Label";
    static constexpr auto icon_name = "Icon";

  private:
    OBJECT_ACCESS_FRIENDS();

    API_AF(AddButton, const char *, initial_text_static, "");
    API_AF(AddButton, Event::Callback, initial_clicked, nullptr);

    AddButton(Object parent, const AddButton &options) {
      m_object = api()->btn_create(parent.object());

      static constexpr lv_coord_t text_height = 50;
      static const auto font
        = Font::find(50, Font::Style::semi_bold).get_font();
      auto container = Container(m_object);
      Container(m_object)
        .set_width(90_percent)
        .set_height(text_height * 2)
        .add(Label(label_name).configure([](Label &label) {
          label.set_alignment(Alignment::left_middle)
            .set_text_font(font)
            .set_width(80_percent);
        }))
        .add(Label(icon_name).configure([](Label &label) {
          label.set_alignment(Alignment::right_middle)
            .set_text_font(font)
            .set_width(20_percent);
        }))
        .add_event_callback(EventCode::clicked, options.initial_clicked());

      container.find(label_name)
        .cast<Label>()
        ->set_text_static(options.initial_text_static());

      container.find(icon_name).cast<Label>()->set_text_static(Icons::plus);
    }
  };

  class Spacer : public lvgl::ObjectAccess<Spacer> {
  public:
    explicit Spacer() : ObjectAccess("") {}

  private:
    OBJECT_ACCESS_FRIENDS();
    Spacer(Object parent, const Spacer &options) {
      m_object = api()->label_create(parent.object());
      api()->label_set_text_static(m_object, "");
      api()->obj_set_width(m_object, LV_PCT(100));
      api()->obj_set_height(m_object, options.initial_height());
    }

    API_AF(Spacer, lv_coord_t, initial_height, 0);
  };

  class Title : public lvgl::ObjectAccess<Title> {
  public:
    explicit Title(const char *name, const char *title)
      : ObjectAccess(name), m_initial_title(title) {}

    static constexpr lv_coord_t height = 120;

  private:
    OBJECT_ACCESS_FRIENDS();
    Title(Object parent, const Title &options) {
      m_object = api()->label_create(parent.object());
      cast<Label>()
        ->set_text_static(options.m_initial_title)
        .set_text_font(Font::find(80, Font::Style::semi_bold))
        .set_width(100_percent)
        .set_height(height);
    }

    const char *m_initial_title;
  };

  class PropertySectionTitle : public lvgl::ObjectAccess<PropertySectionTitle> {
  public:
    explicit PropertySectionTitle(const char *title)
      : ObjectAccess(title), m_initial_title(title) {}

    static constexpr lv_coord_t height = 60;

  private:
    OBJECT_ACCESS_FRIENDS();
    PropertySectionTitle(Object parent, const PropertySectionTitle &options) {
      m_object = api()->label_create(parent.object());
      cast<Label>()
        ->set_text_static(options.m_initial_title)
        .set_text_font(Font::find(50, Font::Style::semi_bold))
        .set_alignment(Alignment::bottom_left)
        .set_width(100_percent)
        .set_height(height);
    }

    const char *m_initial_title;
  };

  class Property : public lvgl::ObjectAccess<Property> {
  public:
    class Context : public Object::Context {
    public:
      Context(lvgl::Property value)
        : Object::Context(Style::to_cstring(value)), m_property(value) {}

    private:
      API_AF(Context, lvgl::Property, property, lvgl::Property::invalid);
    };

    explicit Property(const Context &context)
      : ObjectAccess(context.cast_as_name()) {}

    static constexpr auto label_name = "Label";
    static constexpr auto text_area_name = "TextArea";

  private:
    OBJECT_ACCESS_FRIENDS();

    Property(Object parent, const Property &options) {
      m_object = api()->label_create(parent.object());

      static constexpr lv_coord_t text_height = 50;
      static const auto font = Font::find(50).get_font();

      cast<Label>()->set_text_static("");

      auto container = get<Container>();
      container.set_width(100_percent)
        .set_height(text_height + 20)
        .add(Label(label_name).configure([](Label &label) {
          label.set_text_font(font)
            .set_width(30_percent)
            .set_height(text_height)
            .set_text_alignment(TextAlignment::left)
            .set_alignment(Alignment::left_middle);
        }))
        .add(TextArea(text_area_name).configure([](TextArea &text_area) {
          text_area.set_width(70_percent)
            .clear_flag(TextArea::Flags::scrollable)
            .set_text_font(font)
            .set_text_alignment(TextAlignment::center)
            .set_alignment(Alignment::right_middle)
            .set_one_line_mode()
            .set_height(text_height + 20)
            .add_event_callback(EventCode::defocused, [](lv_event_t *e) {
              const Event event(e);
              const char *text = event.target().cast<TextArea>()->get_text();
              if (Application::model().selected_object ) {
                const Context *c
                  = event.target().get_parent().context<Context>();
                Container(Application::model().selected_object)
                  .set_property(
                    c->property(),
                    PropertyValue(var::StringView(text).to_integer()));
              }
            });
          keyboard_group.add(text_area);
        }));

      const Context *c
        = reinterpret_cast<const Context *>(options.initial_context());

      container.find(label_name)
        .cast<Label>()
        ->set_text_static(Style::to_cstring(c->property()));
    }
  };

  class Card : public lvgl::ObjectAccess<Card> {
  public:
    explicit Card(const char *name = "") : ObjectAccess(name) {}

  private:
    OBJECT_ACCESS_FRIENDS();
    Card(Object parent, const Card &options) {
      m_object = api()->btn_create(parent.object());

      Container container(m_object);

      static constexpr lv_coord_t height = 150;
      static constexpr lv_coord_t icon_height = 72;
      static constexpr lv_coord_t icon_offset = (height - icon_height) / 2;
      static const auto icon_font = Font::find("montserrat", icon_height);

      container.set_vertical_padding(0)
        .set_horizontal_padding(20)
        .set_width(95_percent)
        .set_alignment(Alignment::center)
        .set_height(height)
        .add(Label("label").configure([](Label &object) {
          object.set_y(icon_offset)
            .set_text_font(icon_font)
            .set_width(90_percent)
            .set_height(95_percent);
        }))
        .add(Label(Icons::chevron_right).configure([](Label &object) {
          object.set_y(icon_offset)
            .set_text_font(icon_font)
            .set_x(90_percent)
            .set_width(10_percent)
            .set_text_alignment(TextAlignment::right)
            .set_height(icon_height)
            .set_text_static(Icons::chevron_right);
        }));

      container.find("label").cast<Label>()->set_text(options.initial_icon());
    }

    API_AF(Card, const char *, initial_icon, "");
  };

  class Navigation : public lvgl::ObjectAccess<Navigation> {
  public:
    explicit Navigation() : ObjectAccess("") {}

  private:
    OBJECT_ACCESS_FRIENDS();
    Navigation(Object parent, const Navigation &) {
      m_object = api()->obj_create(parent.object());

      Container container(m_object);
      container.set_vertical_padding(0)
        .set_width(95_percent)
        .set_alignment(Alignment::center)
        .set_height(10_percent)
        .add(Button(Icons::chevron_left).configure([](Button &button) {
          button.set_x(2_percent)
            .set_width(15_percent)
            .set_height(95_percent)
            .add_flag(Flags::event_bubble)
            .add_label();
        }))
        .add(Button(Icons::chevron_right).configure([](Button &button) {
          button.set_x(83_percent)
            .set_width(15_percent)
            .set_height(95_percent)
            .add_flag(Flags::event_bubble)
            .add_label();
        }));
    }
  };
};

using E = Elements;

} // namespace lvgl

#endif // ELEMENTS_HPP
