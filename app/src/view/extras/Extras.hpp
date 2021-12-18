//
// Created by Tyler Gilbert on 12/14/21.
//

#ifndef DESIGNLAB_EXTRAS_HPP
#define DESIGNLAB_EXTRAS_HPP

#include <design/Typography.hpp>
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
    Event::Callback callback);
};

class FormHeaderRow : public ObjectAccess<FormHeaderRow> {
public:
  FormHeaderRow(const char *title, Event::Callback callback);
};

class AddButtonRow : public ObjectAccess<AddButtonRow> {
public:
  AddButtonRow(const char * button_name, const char * icon, const char *title, Event::Callback callback);
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
  InfoCard(lv_obj_t * object){ m_object = object; }
  static InfoCard get_from_member(lvgl::Object object);
  static InfoCard::Data* get_data_from_event(lv_event_t * e);

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
  IconCheck(u16 icon, var::StringView name, var::StringView unicode);
  IconCheck(lv_obj_t*object){ m_object = object; }

  IconCheck& set_checked(bool value = true){
    auto button = find<lvgl::Button>(Names::checkable_button);
    if( value ){
      button.add_state(State::checked);
    } else {
      button.clear_state(State::checked);
    }
    return *this;
  }

private:
  struct Names {
    static constexpr auto checkable_button = "CheckButton";
  };

};

class IconGrid : public ObjectAccess<IconGrid> {
public:
  LVGL_OBJECT_ACCESS_DECLARE_CONSTRUCTOR(IconGrid);
};

class IconGridContainer : public ObjectAccess<IconGridContainer> {
public:
  LVGL_OBJECT_ACCESS_DECLARE_CONSTRUCTOR(IconGridContainer);

  static constexpr size_t max_icon_count = 100;

  IconGrid get_icon_grid() const {
    return find<IconGrid>(Names::icon_grid);
  }
private:
  struct Names {
    static constexpr auto icon_grid = "IconGrid";
  };
};

#endif // DESIGNLAB_EXTRAS_HPP
