//
// Created by Tyler Gilbert on 3/1/22.
//

#ifndef DESIGNLAB_ADDCOMPONENT_HPP
#define DESIGNLAB_ADDCOMPONENT_HPP

#include <design/extras/Form.hpp>
#include <design/macros.hpp>
#include <lvgl/ObjectAccess.hpp>

#include "designlab/fonts/FontAwesomeIcons.hpp"

class AddComponent : public lvgl::ObjectAccess<AddComponent> {
public:
  LVGL_OBJECT_ACCESS_DECLARE_CONSTRUCTOR(AddComponent);

  AddComponent &set_parent_item_name(const char *value);

  design::Form get_form() {
    return find<design::Form>({Names::form_column, Names::form});
  }

  struct Fields {
    DESIGN_DECLARE_NAME(component_name);
    DESIGN_DECLARE_NAME(component_type);
    DESIGN_DECLARE_NAME_VALUE(component_width, width);
    DESIGN_DECLARE_NAME_VALUE(component_height, height);

    DESIGN_DECLARE_NAME(component_button_label);
    DESIGN_DECLARE_NAME(component_heading1_label);
    DESIGN_DECLARE_NAME(component_heading2_label);
    DESIGN_DECLARE_NAME(component_heading3_label);
    DESIGN_DECLARE_NAME(component_heading4_label);

    DESIGN_DECLARE_NAME(component_form_line_field_label);
    DESIGN_DECLARE_NAME(component_form_line_field_hint);

    DESIGN_DECLARE_NAME(component_form_select_label);
    DESIGN_DECLARE_NAME(component_form_select_hint);

    DESIGN_DECLARE_NAME(component_form_file_select_label);
    DESIGN_DECLARE_NAME(component_form_file_select_hint);

    DESIGN_DECLARE_NAME(component_form_switch_label);
    DESIGN_DECLARE_NAME(component_form_switch_hint);
  };

  struct Components {
    DESIGN_DECLARE_NAME(bar);
    DESIGN_DECLARE_NAME(button);
    DESIGN_DECLARE_NAME(calendar);
    DESIGN_DECLARE_NAME(canvas);
    DESIGN_DECLARE_NAME(chart);
    DESIGN_DECLARE_NAME(color_wheel);
    DESIGN_DECLARE_NAME(image);
    DESIGN_DECLARE_NAME(keyboard);
    DESIGN_DECLARE_NAME(label);
    DESIGN_DECLARE_NAME(list);
    DESIGN_DECLARE_NAME(meter);
    DESIGN_DECLARE_NAME(roller);
    DESIGN_DECLARE_NAME(slider);
    DESIGN_DECLARE_NAME(spinbox);
    DESIGN_DECLARE_NAME(spinner);
    DESIGN_DECLARE_NAME(switch_);
    DESIGN_DECLARE_NAME(table);
    DESIGN_DECLARE_NAME(tileview);
    DESIGN_DECLARE_NAME(window);

    static constexpr auto list_of_lvgl_components = {
      bar,
      button,
      calendar,
      canvas,
      chart,
      color_wheel,
      image,
      keyboard,
      label,
      list,
      meter,
      roller,
      slider,
      spinbox,
      spinner,
      switch_,
      table,
      tileview,
      window};

    DESIGN_DECLARE_NAME(alert);
    DESIGN_DECLARE_NAME(badge);
    DESIGN_DECLARE_NAME(card);
    DESIGN_DECLARE_NAME(column);
    DESIGN_DECLARE_NAME(container);
    DESIGN_DECLARE_NAME(drawer);
    DESIGN_DECLARE_NAME(heading1);
    DESIGN_DECLARE_NAME(heading2);
    DESIGN_DECLARE_NAME(heading3);
    DESIGN_DECLARE_NAME(heading4);
    DESIGN_DECLARE_NAME(horizontal_line);
    DESIGN_DECLARE_NAME(paragraph);
    DESIGN_DECLARE_NAME(row);

    static constexpr auto list_of_design_components = {
      alert,
      badge,
      card,
      column,
      container,
      drawer,
      heading1,
      heading2,
      heading3,
      heading4,
      horizontal_line,
      paragraph,
      row};

    DESIGN_DECLARE_NAME(check_list);
    DESIGN_DECLARE_NAME(drawer_stack);
    DESIGN_DECLARE_NAME(filesystem_card);
    DESIGN_DECLARE_NAME(form_container);
    DESIGN_DECLARE_NAME(form_heading);
    DESIGN_DECLARE_NAME(form_line_field);
    DESIGN_DECLARE_NAME(form_list);
    DESIGN_DECLARE_NAME(form_select);
    DESIGN_DECLARE_NAME(form_file_select);
    DESIGN_DECLARE_NAME(form_switch);
    DESIGN_DECLARE_NAME(notification_toast);
    DESIGN_DECLARE_NAME(prompt);

    static constexpr auto list_of_extra_design_components = {
      check_list,
      drawer_stack,
      filesystem_card,
      form_container,
      form_heading,
      form_line_field,
      form_list,
      form_select,
      form_file_select,
      form_switch,
      notification_toast,
      prompt};
  };

  static bool is_cancel_button(lv_event_t *e);

private:
  struct ComponentType {
    const char *name;
    const char *icon;
  };

  struct Names {
    DESIGN_DECLARE_NAME(top_container);
    DESIGN_DECLARE_NAME(parent_label);
    DESIGN_DECLARE_NAME(get_parent_button);
    DESIGN_DECLARE_NAME(get_previous_sibling_button);
    DESIGN_DECLARE_NAME(get_next_sibling_button);
    DESIGN_DECLARE_NAME(add_button);
    DESIGN_DECLARE_NAME(object_dropdown);
    DESIGN_DECLARE_NAME(column);
    DESIGN_DECLARE_NAME(form);
    DESIGN_DECLARE_NAME(fill_width_button);
    DESIGN_DECLARE_NAME(fill_height_button);
    DESIGN_DECLARE_NAME(cancel_button);

    DESIGN_DECLARE_NAME(main_row);
    DESIGN_DECLARE_NAME(control_column);
    DESIGN_DECLARE_NAME(form_column);
    DESIGN_DECLARE_NAME(form_hline_type_start);
    DESIGN_DECLARE_NAME(form_hline_type_stop);

    DESIGN_DECLARE_NAME(lvgl_row);
    DESIGN_DECLARE_NAME(design_row);
    DESIGN_DECLARE_NAME(extra_design_row);
    DESIGN_DECLARE_NAME(extra_design_form_row);
  };

  static constexpr auto component_type_list_lvgl = {
    ComponentType{Components::bar, icons::fa::spinner_solid},
    ComponentType{Components::button, icons::fa::calendar_alt_solid},
    ComponentType{Components::calendar, icons::fa::calendar_alt_solid},
    ComponentType{Components::canvas, icons::fa::paint_brush_solid},
    ComponentType{Components::chart, icons::fa::chart_line_solid},
    ComponentType{Components::color_wheel, icons::fa::palette_solid},
    ComponentType{Components::image, icons::fa::image_solid},
    ComponentType{Components::keyboard, icons::fa::keyboard_solid},
    ComponentType{Components::label, icons::fa::info_solid},
    ComponentType{Components::list, icons::fa::list_alt_solid},
    ComponentType{Components::meter, icons::fa::tachometer_alt_solid},
    ComponentType{Components::roller, icons::fa::tasks_solid},
    ComponentType{Components::slider, icons::fa::sliders_h_solid},
    ComponentType{Components::spinbox, icons::fa::indent_solid},
    ComponentType{Components::spinner, icons::fa::spinner_solid},
    ComponentType{Components::switch_, icons::fa::toggle_on_solid},
    ComponentType{Components::table, icons::fa::table_solid},
    ComponentType{Components::tileview, icons::fa::th_solid},
    ComponentType{Components::window, icons::fa::window_maximize_solid},
  };

  static constexpr auto component_type_list_design = {
    ComponentType{Components::alert, icons::fa::exclamation_triangle_solid},
    ComponentType{Components::badge, icons::fa::certificate_solid},
    ComponentType{Components::card, icons::fa::square_solid},
    ComponentType{Components::column, icons::fa::columns_solid},
    ComponentType{Components::container, icons::fa::arrows_alt_solid},
    ComponentType{Components::drawer, icons::fa::palette_solid},
    ComponentType{Components::heading1, icons::fa::heading_solid},
    ComponentType{Components::heading2, icons::fa::heading_solid},
    ComponentType{Components::heading3, icons::fa::heading_solid},
    ComponentType{Components::heading4, icons::fa::heading_solid},
    ComponentType{
      Components::horizontal_line,
      icons::fa::window_minimize_solid},
    ComponentType{Components::paragraph, icons::fa::align_justify_solid},
    ComponentType{Components::row, icons::fa::stream_solid}};

  static constexpr auto component_type_list_extra_design = {
    ComponentType{Components::check_list, icons::fa::check_double_solid},
    ComponentType{Components::drawer_stack, icons::fa::calendar_alt_solid},
    ComponentType{Components::filesystem_card, icons::fa::folder_open_solid},
    ComponentType{Components::form_container, icons::fa::tasks_solid},
    ComponentType{Components::form_list, icons::fa::clipboard_list_solid},
    ComponentType{
      Components::notification_toast,
      icons::fa::exclamation_circle_solid},
    ComponentType{Components::prompt, icons::fa::question_circle_solid},
  };

  static constexpr auto component_type_list_extra_design_form = {
    ComponentType{Components::form_heading, icons::fa::heading_solid},
    ComponentType{Components::form_line_field, icons::fa::terminal_solid},
    ComponentType{Components::form_select, icons::fa::calendar_alt_solid},
    ComponentType{Components::form_file_select, icons::fa::file_alt_solid},
    ComponentType{Components::form_switch, icons::fa::toggle_on_solid},
  };

  static constexpr auto component_type_list_extra_design_form_list
    = {ComponentType{Components::form_heading, icons::fa::heading_solid}};

  void add_type_selection(lvgl::Generic generic);

  static void form_clicked(lv_event_t *e);
  static void fill_width_clicked(lv_event_t *e);
  static void fill_height_clicked(lv_event_t *e);
  static void control_button_clicked(lv_event_t *e);

  static AddComponent get_add_component(lv_event_t *e);
};

#endif // DESIGNLAB_ADDCOMPONENT_HPP
