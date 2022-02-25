//
// Created by Tyler Gilbert on 12/29/21.
//
#include <var.hpp>

#include "designlab/fonts/FontAwesomeIcons.hpp"

#include "About.hpp"
#include "extras/Extras.hpp"

About::About(const char *name) {
  construct_object(name);
  fill();

  clear_flag(Flags::scrollable)
    .add(Container(ViewObject::Names::content_container)
           .fill()
           .add(Column(Names::about_column)));

  auto column = find<Column>(Names::about_column);
  column.fill()
    .add(HeaderRow(
      "About",
      icons::fa::info_circle_solid,
      "Check for Updates",
      nullptr))
    .add(HorizontalLine());
  static const auto design_lab_section
    = KeyString(icons::fa::flask_solid).append(" Design Lab");
  column.add(SectionHeading(design_lab_section))
    .add(AttributionRow(
      "Publisher",
      "Stratify Labs, Inc",
      "https://stratifylabs.dev/"))
    .add(AttributionRow("Version", VERSION))
    .add(HorizontalLine().set_height(20).add_style("bg_warning"));

  static const auto credits_section
    = KeyString(icons::fa::align_justify_solid).append(" Credits");

  column.add(SectionHeading(credits_section))
    .add(AttributionRow("Icons", "FontAwesome", "https://fontawesome.com/"))
    .add(AttributionRow("Graphics", "LVGL", "https://lvgl.io"))
    .add(AttributionRow("Window", "SDL", "https://www.libsdl.org/"))
    .add(
      AttributionRow("JSON", "jansson", "https://github.com/akheron/jansson"))
    .add(AttributionRow(
      "Framework",
      "Stratify Labs API",
      "https://github.com/StratifyLabs/API"));

  column.add(HorizontalLine().set_height(20).add_style("bg_info"));

  static const auto code_section
    = KeyString(icons::fa::code_solid).append(" Code");

  column.add(SectionHeading(code_section))
    .add(AttributionRow(
      "Repository",
      "Github",
      "https://github.com/StratifyLabs/DesignLab"));

  column.add(HorizontalLine().set_height(20).add_style("bg_danger"));

  column.add(SectionHeading("Theme"))
    .add(Row()
           .fill_width()
           .add(Button(Names::dark_button)
                  .add_label(KeyString(icons::fa::moon_solid).append(" Dark"))
                  .add_style("btn_dark")
                  .add_event_callback(EventCode::clicked, update_theme))
           .add(Button(Names::light_button)
                  .add_label(KeyString(icons::fa::sun_solid).append(" Light"))
                  .add_style("btn_warning")
                  .add_event_callback(EventCode::clicked, update_theme)));
}

void About::update_theme(lv_event_t *e) {
  NotifyHome notify_home;
  Model::Scope model_scope;
  const auto is_dark = Event(e).target().name() == Names::dark_button;
  const auto &theme = is_dark ? model().dark_theme : model().light_theme;
  Display(model().runtime->display()).set_theme(theme);
  screen().update_layout().invalidate();
  model().is_dark_theme = is_dark;
  model().is_theme_updated = true;
  model().session_settings.set_theme(is_dark ? "dark" : "light");
}
