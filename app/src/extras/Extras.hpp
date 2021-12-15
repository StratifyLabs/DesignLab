//
// Created by Tyler Gilbert on 12/14/21.
//

#ifndef DESIGNLAB_EXTRAS_HPP
#define DESIGNLAB_EXTRAS_HPP

#include <design/Typography.hpp>
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
  FormHeaderRow(
    const char *title,
    Event::Callback callback);
};

class AddButtonRow : public ObjectAccess<AddButtonRow> {
public:
  AddButtonRow(
    const char *title,
    Event::Callback callback);
};

#endif // DESIGNLAB_EXTRAS_HPP
