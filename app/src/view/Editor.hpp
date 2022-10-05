//
// Created by Tyler Gilbert on 12/17/21.
//

#ifndef DESIGNLAB_EDITOR_HPP
#define DESIGNLAB_EDITOR_HPP

#include <design/Typography.hpp>
#include <json/Json.hpp>
#include <design/extras/Form.hpp>

#include "extras/Extras.hpp"
#include "model/Model.hpp"

class Editor : public ObjectAccess<Editor>, public api::ExecutionContext {
public:

  enum class IsValid {
    no, yes
  };

  using ValidateCallback = IsValid (*)(design::Form);
  using EditClickedCallback = void (*)(lv_event_t * e, u32 offset);
  using AddClickedCallback = void (*)(lv_event_t * e);

  using GetFormSchemaCallback = design::Form::Schema (*)();
  using GetFeatureListCallback
    = var::Vector<InfoCard::Data::Feature> (*)(json::JsonObject);
  using GetInfoTitleCallback = var::StringView (*)(json::JsonObject);

  class Data : public UserDataAccess<Data> {
  public:
    explicit Data(const char * name = "") : UserDataAccess<Data>(name){}

    API_PMAZ(add_button_text, Data, const char *, "Add Button");
    API_PMAZ(add_clicked_callback, Data, AddClickedCallback, nullptr);
    API_PMAZ(edit_clicked_callback, Data, EditClickedCallback, nullptr);
    API_PMAZ(form_name, Data, const char *, "");
    API_PMAZ(form_title, Data, const char *, "Form Title");
    API_PMAZ(get_feature_list_callback, Data, GetFeatureListCallback, nullptr);
    API_PMAZ(get_info_title_callback, Data, GetInfoTitleCallback, nullptr);
    API_PMAZ(get_schema_callback, Data, GetFormSchemaCallback, nullptr);
    API_PMAZ(nothing_to_show, Data, const char *, "No Items");
    API_PMAZ(title, Data, const char *, "Title");
    API_PMAZ(validate_callback, Data, ValidateCallback, nullptr);

    API_PMAZ(edit_offset, Data, u32, 0);

  private:
    friend Editor;
    API_PMAZ(input_form_schema, Data, design::Form::Schema, {});
  };

  explicit Editor(Data &data);
  explicit Editor(lv_obj_t*object){ m_object = object; }

  Editor& refresh_values();

protected:
  struct Names {
    static constexpr auto first_child_column = "EditorFirstChildColumn";
    static constexpr auto input_form_container = "InputFormContainer";
    static constexpr auto input_form_column = "InputFormColumn";
    static constexpr auto show_column = "showColumn";
    static constexpr auto edit_button = "editButton";
    static constexpr auto add_button = "addButton";
  };

  void configure_form_input(Data &data);

};

#endif // DESIGNLAB_EDITOR_HPP
