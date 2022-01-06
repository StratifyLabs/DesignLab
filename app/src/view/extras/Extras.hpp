//
// Created by Tyler Gilbert on 12/14/21.
//

#ifndef DESIGNLAB_EXTRAS_HPP
#define DESIGNLAB_EXTRAS_HPP

#include <design/Typography.hpp>
#include <design/macros.hpp>
#include <json/Json.hpp>
#include <lvgl.hpp>

using ScreenHeading = design::Heading1;
using SectionHeading = design::Heading2;
using SubSectionHeading = design::Heading3;

class HeaderRow : public ObjectAccess<HeaderRow> {
public:
  HeaderRow(
    const char *title,
    const char *button_icon,
    const char *button_text,
    Event::Callback callback,
    const char *color = "btn_success");
  HeaderRow(lv_obj_t *object) { m_object = object; }

  Button get_button() const { return find<Button>(Names::action_button); }

private:
  struct Names {
    static constexpr auto action_button = "ActionButton";
  };
};

class AttributionRow : public ObjectAccess<AttributionRow> {
public:
  AttributionRow(
    const char *name,
    const char *description,
    const char *external_link = nullptr);
  AttributionRow(lv_obj_t *object) { m_object = object; }


private:
  struct Names {
    DESIGN_DECLARE_NAME(external_link_button);
    DESIGN_DECLARE_NAME(link_row);
    DESIGN_DECLARE_NAME(dots_label);
  };
};

class ColorSlider : public ObjectAccess<ColorSlider> {
public:
  ColorSlider(
    const char *name,
    u16 maximum,
    void (*value_changed)(lv_event_t *));
  ColorSlider(lv_obj_t *object) { m_object = object; }

  u8 get_u8() const {
    return var::StringView(find<TextArea>(Names::value_text_area).get_text())
      .to_unsigned_long(var::StringView::Base::auto_);
  }

  u16 get_u16() const {
    return var::StringView(find<TextArea>(Names::value_text_area).get_text())
      .to_unsigned_long(var::StringView::Base::auto_);
  }

  ColorSlider &set_u8(u8 value) {
    auto slider = find<Slider>(Names::value_slider);
    if (slider.get_value() != value) {
      find<TextArea>(Names::value_text_area)
        .set_text(var::NumberString(value, "0x%02x"));
      slider.set_value(value);
    }
    return *this;
  }

  ColorSlider &set_u16(u16 value) {
    auto slider = find<Slider>(Names::value_slider);
    if (slider.get_value() != value) {
      find<TextArea>(Names::value_text_area)
        .set_text(var::NumberString(value, "0x%04x"));
      slider.set_value(value);
    }
    return *this;
  }

private:
  struct Names {
    DESIGN_DECLARE_NAME(value_text_area);
    DESIGN_DECLARE_NAME(value_slider);
    DESIGN_DECLARE_NAME(label_row);
  };
};

class ColorButton : public ObjectAccess<ColorButton> {
public:
  ColorButton(
    const char *name,
    Color color,
    void (*clicked)(lv_event_t*));
  ColorButton(lv_obj_t *object) { m_object = object; }

  ColorButton & set_color(Color color);

private:
  struct Names {
    DESIGN_DECLARE_NAME(color_label);
  };
};

class FormHeaderRow : public ObjectAccess<FormHeaderRow> {
public:
  FormHeaderRow(const char *title, Event::Callback callback);
};

class AddButtonRow : public ObjectAccess<AddButtonRow> {
public:
  AddButtonRow(
    const char *button_name,
    const char *icon,
    const char *title,
    Event::Callback callback);
};

class InfoCard : public ObjectAccess<InfoCard> {
public:
  class Data : public UserDataAccess<Data> {
  public:
    explicit Data(const char *name = "") : UserDataBase(name) {}

    API_PMAZ(animation_time, Data, chrono::MicroTime, chrono::MicroTime{});
    API_PMAZ(back_symbol, Data, const char *, LV_SYMBOL_UP);
    API_PMAZ(base_path, Data, var::PathString, {});
    API_PMAZ(description, Data, var::PathString, {});
    API_PMAZ(directory_symbol, Data, const char *, LV_SYMBOL_DIRECTORY);
    API_PMAZ(edit_callback, Data, Event::Callback, nullptr);
    API_PMAZ(file_symbol, Data, const char *, LV_SYMBOL_FILE);

    // bool members start with `is_`
    API_PUBLIC_BOOL(Data, select_file, false);

    API_PMAZ(offset, Data, u32, 0);
    API_PMAZ(remove_callback, Data, Event::Callback, nullptr);
    API_PMAZ(title, Data, var::KeyString, {});

    struct Feature {
      const char *icon;
      const char *label;
      var::KeyString value;
    };

    Data &push_feature(const Feature &feature) {
      m_feature_list.push_back(feature);
      return *this;
    }

    const var::Vector<Feature> &feature_list() const { return m_feature_list; }

  private:
    var::Vector<Feature> m_feature_list;
  };

  InfoCard(Data &data);
  InfoCard(lv_obj_t *object) { m_object = object; }
  static InfoCard get_from_member(lvgl::Object object);
  static InfoCard::Data *get_data_from_event(lv_event_t *e);

private:
  struct Names {
    static constexpr auto card = "Card";
    static constexpr auto card_body_row = "CardBodyRow";
    static constexpr auto card_body_left_column = "CardBodyLeftCol";
    static constexpr auto edit_button = "EditButton";
    static constexpr auto remove_button = "RemoveButton";
    static constexpr auto button_row = "CardButtonRow";
  };
};

class IconCheck : public ObjectAccess<IconCheck> {
public:
  IconCheck(
    u16 icon_unicode,
    const var::StringView name,
    var::StringView family,
    Font font);
  IconCheck(lv_obj_t *object) { m_object = object; }

  const char *get_name() const;
  const char *get_unicode() const;
  const char *get_family() const;

  IconCheck &set_checked(bool value = true);
  bool is_checked() const;

private:
  struct Names {
    static constexpr auto checkable_button = "CheckButton";
    static constexpr auto name_label = "NameLabel";
    static constexpr auto family_label = "FamilyLabel";
    static constexpr auto unicode_label = "UnicodeLabel";
  };
};

class IconGrid : public ObjectAccess<IconGrid> {
public:
  LVGL_OBJECT_ACCESS_DECLARE_CONSTRUCTOR(IconGrid);
};

class IconGridContainer : public ObjectAccess<IconGridContainer> {
public:
  LVGL_OBJECT_ACCESS_DECLARE_CONSTRUCTOR(IconGridContainer);

  static constexpr size_t max_icon_count = 88;

  IconGrid get_icon_grid() const { return find<IconGrid>(Names::icon_grid); }

private:
  struct Names {
    static constexpr auto icon_grid = "IconGrid";
  };
};

#endif // DESIGNLAB_EXTRAS_HPP
