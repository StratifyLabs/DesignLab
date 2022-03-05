//
// Created by Tyler Gilbert on 12/29/21.
//
#include <var.hpp>
#include <sys/System.hpp>

#include "designlab/fonts/FontAwesomeIcons.hpp"

#include "GettingStarted.hpp"
#include "extras/Extras.hpp"

static constexpr auto introduction_paragraph
  = R"(Welcome to Design Lab! Design lab is a desktop application
used to generate themes and assets for LVGL based programs
that run on both microcontrollers and desktop operating
systems including Windows, macOS, and Linux.

You will want to start with the Stratify Labs gui template project on github.
You can click the green button above to open Github in a browser.)";

static constexpr auto project_paragraph
  = R"(The first thing you need to do is configure your project. You need to decide
where your project will be located (directory) and where to generate code within
your project.

Design Lab will generate a folder called 'designlab' within the 'Source Directory'.
Within the 'designlab' folder, folders will be created for: 'assets', 'components', 'fonts',
and 'themes'.

The 'assets' folder will contain a binary file that is a concatenation of the assets plus
some header information.

The 'components' folder is for '.cpp' and '.hpp' files that are generated from
the components you build in Design Lab.

The 'fonts' folder is where the '.c' files for generated fonts will reside.

The 'themes' folder is where '.c' files will be generated from the theme input files.

Design Lab also generates a 'CMakeLists.txt' file to make it easy to build the
source files within a `cmake` project.
)";

static constexpr auto theme_paragraph
  = R"(Themes are defined using a series of JSON files that can be mixed and matched
if you need to create similar themes (like light and dark).

There is a main JSON file which references the JSON files you can use to define variables
for sizes, fonts, styles, and rules.)";

static constexpr auto asset_paragraph
  = R"(Assets...)";

static constexpr auto font_paragraph
  = R"(Fonts...)";

static constexpr auto icon_paragraph
  = R"(Icons...)";

static constexpr auto component_paragraph
  = R"(Components...)";

static constexpr auto color_paragraph
  = R"(Colors...)";

GettingStarted::GettingStarted(const char *name) {
  construct_object(name);
  fill();

  clear_flag(Flags::scrollable)
    .add(Container(ViewObject::Names::content_container)
           .fill()
           .add(Column(Names::main_column)));

  auto column = find<Column>(Names::main_column);
  column.fill()
    .add(
      HeaderRow("Getting Started", icons::fa::github_brands, "gui", [](lv_event_t*){
        sys::System::launch_browser("https://github.com/StratifyLabs/gui");
      }))
    .add(HorizontalLine());

  {
    column.add(Heading2("Introduction"));
    static const auto text = remove_newlines(introduction_paragraph);
    column.add(Paragraph(Names::introduction_paragraph, text.cstring()));
  }

  {
    column.add(Heading2("Project"));
    static const auto text = remove_newlines(project_paragraph);
    column.add(Paragraph(Names::project_paragraph, text.cstring()));
  }

  {
    column.add(Heading2("Themes"));
    static const auto text = remove_newlines(theme_paragraph);
    column.add(Paragraph(Names::theme_paragraph, text.cstring()));
  }

  {
    column.add(Heading2("Assets"));
    static const auto text = remove_newlines(asset_paragraph);
    column.add(Paragraph(Names::asset_paragraph, text.cstring()));
  }

  {
    column.add(Heading2("Fonts"));
    static const auto text = remove_newlines(font_paragraph);
    column.add(Paragraph(Names::font_paragraph, text.cstring()));
  }

  {
    column.add(Heading2("Icons"));
    static const auto text = remove_newlines(icon_paragraph);
    column.add(Paragraph(Names::icon_paragraph, text.cstring()));
  }

  {
    column.add(Heading2("Components"));
    static const auto text = remove_newlines(component_paragraph);
    column.add(Paragraph(Names::component_paragraph, text.cstring()));
  }

  {
    column.add(Heading2("Colors"));
    static const auto text = remove_newlines(color_paragraph);
    column.add(Paragraph(Names::color_paragraph, text.cstring()));
  }
}

var::String GettingStarted::remove_newlines(const char *paragraph) {
  return String(paragraph)
    .replace(String::Replace().set_old_string("\n\n").set_new_string("__"))
    .replace(String::Replace().set_old_string("\n").set_new_string(" "))
    .replace(String::Replace().set_old_string("__").set_new_string("\n\n"));
}
