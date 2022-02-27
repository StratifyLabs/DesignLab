//
// Created by Tyler Gilbert on 2/24/22.
//

#ifndef DESIGNLAB_BUILDERTOOLS_HPP
#define DESIGNLAB_BUILDERTOOLS_HPP

#include <design/extras/Form.hpp>
#include <design/macros.hpp>
#include <lvgl/ObjectAccess.hpp>

class BuilderTools : public lvgl::ObjectAccess<BuilderTools> {
public:
  LVGL_OBJECT_ACCESS_DECLARE_CONSTRUCTOR(BuilderTools);

  design::Form get_add_component_form() {
    return find<design::Form>(Names::form);
  }

  struct Fields {
    DESIGN_DECLARE_NAME(component_name);
    DESIGN_DECLARE_NAME(component_type);
    DESIGN_DECLARE_NAME(component_width);
    DESIGN_DECLARE_NAME(component_height);
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
    DESIGN_DECLARE_NAME(toast);

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
      row,
      toast};

    DESIGN_DECLARE_NAME(check_list);
    DESIGN_DECLARE_NAME(drawer_stack);
    DESIGN_DECLARE_NAME(filesystem_card);
    DESIGN_DECLARE_NAME(form);
    DESIGN_DECLARE_NAME(form_heading);
    DESIGN_DECLARE_NAME(form_line_field);
    DESIGN_DECLARE_NAME(form_list);
    DESIGN_DECLARE_NAME(form_select);
    DESIGN_DECLARE_NAME(form_select_file);
    DESIGN_DECLARE_NAME(form_switch);
    DESIGN_DECLARE_NAME(notification_toast);
    DESIGN_DECLARE_NAME(prompt);

    static constexpr auto list_of_extra_design_components = {
      check_list,
      drawer_stack,
      filesystem_card,
      form,
      form_heading,
      form_line_field,
      form_list,
      form_select,
      form_select_file,
      form_switch,
      notification_toast,
      prompt};
  };

private:
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
  };

  static void form_clicked(lv_event_t *e);
  static void fill_width_clicked(lv_event_t *e);
  static void fill_height_clicked(lv_event_t *e);

  static BuilderTools get_builder_tools(lv_event_t *e);
};

#endif // DESIGNLAB_BUILDERTOOLS_HPP
