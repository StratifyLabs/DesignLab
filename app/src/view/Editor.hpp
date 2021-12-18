//
// Created by Tyler Gilbert on 12/17/21.
//

#ifndef DESIGNLAB_EDITOR_HPP
#define DESIGNLAB_EDITOR_HPP

#include <design/Typography.hpp>
#include <json/Json.hpp>
#include <lvgl.hpp>
#include <design.hpp>

#include <design/extras/Form.hpp>

#include "extras/Extras.hpp"
#include "model/Model.hpp"

class Editor : public ObjectAccess<Editor>, public ModelAccess {
public:
  using GetFormSchemaCallback = design::Form::Schema (*)();
  using GetFeatureListCallback
    = var::Vector<InfoCard::Data::Feature> (*)(json::JsonObject);
  using GetInfoTitleCallback = var::StringView (*)(json::JsonObject);

  class Data : public UserDataAccess<Data> {
  public:


    API_PMAZ(form_name, Data, const char *, "");
    API_PMAZ(get_schema_callback, Data, GetFormSchemaCallback, nullptr);
    API_PMAZ(get_feature_list_callback, Data, GetFeatureListCallback, nullptr);
    API_PMAZ(get_info_title_callback, Data, GetInfoTitleCallback, nullptr);
    API_PMAZ(title, Data, const char *, "Title");
    API_PMAZ(form_title, Data, const char *, "Form Title");
    API_PMAZ(nothing_to_show, Data, const char *, "No Items");
    API_PMAZ(add_button_text, Data, const char *, "Add Button");

  private:
    friend Editor;
    API_PMAZ(input_form_schema, Data, design::Form::Schema, {});
    API_PMAZ(edit_offset, Data, u32, 0);
  };

  explicit Editor(Data &data);
  explicit Editor(lv_obj_t*object){ m_object = object; }

private:
  enum class Action { add, edit };

  struct Names {
    static constexpr auto first_child_column = "EditorFirstChildColumn";
    static constexpr auto input_form_container = "InputFormContainer";
    static constexpr auto input_form_column = "InputFormColumn";
    static constexpr auto show_column = "showColumn";
    static constexpr auto edit_button = "editButton";
    static constexpr auto add_button = "addButton";
  };

  void show_values(Data &data);
  void configure_form_input(Data &data);

  static void hide_form(lv_event_t *) {
    screen()
      .find<Container>(Names::input_form_container)
      .clear_state(State::user1);
  }

  static void show_add_form(lv_event_t *e);


  static void remove_entry(const char *key_name, u32 offset) {
    Model::Scope model_scope;
    auto array = model().project_settings.to_object().at(key_name).to_array();
    if (offset < array.count()) {
      array.remove(offset);
    }
  }

  static Editor get_self(lv_event_t *e) {
    const auto target = Event(e).target();
    auto column_parent = target.find_parent<Column>(Names::first_child_column);
    auto form_container_parent = target.find_parent<Column>(Names::input_form_container);
    auto editor = column_parent.is_valid() ?  column_parent.get_parent() : form_container_parent.get_parent();
    return editor.get<Editor>();
  }

  static Data *get_data(lv_event_t *e) {
    return get_self(e).user_data<Data>();
  }

  void set_action(Action action) {
    if (action == Action::add) {
      find<Button>(Names::add_button).clear_flag(Flags::hidden);
      find<Button>(Names::edit_button).add_flag(Flags::hidden);
    } else {
      // edit
      find<Button>(Names::add_button).add_flag(Flags::hidden);
      find<Button>(Names::edit_button).clear_flag(Flags::hidden);
    }
  }

  void show_form() {
    find<Container>(Names::input_form_container).add_state(State::user1);
    find<Column>(Names::input_form_column).scroll_to_y(0, IsAnimate::no);
  }


  static void submit_form_add(lv_event_t *e);
  static void submit_form_edit(lv_event_t *e);
  static void edit_info_card(lv_event_t *e);
  static void remove_info_card(lv_event_t *e);
};

#endif // DESIGNLAB_EDITOR_HPP
