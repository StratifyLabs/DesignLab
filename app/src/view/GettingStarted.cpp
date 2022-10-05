//
// Created by Tyler Gilbert on 12/29/21.
//
#include <var.hpp>
#include <sys/System.hpp>

#include "designlab/fonts/FontAwesomeIcons.hpp"

#include "GettingStarted.hpp"
#include "extras/Extras.hpp"

namespace {

struct LocalNames {
  DESIGN_DECLARE_NAME(main_column);
  DESIGN_DECLARE_NAME(introduction_paragraph);
  DESIGN_DECLARE_NAME(project_paragraph);
  DESIGN_DECLARE_NAME(theme_paragraph);
  DESIGN_DECLARE_NAME(asset_paragraph);
  DESIGN_DECLARE_NAME(font_paragraph);
  DESIGN_DECLARE_NAME(icon_paragraph);
  DESIGN_DECLARE_NAME(component_paragraph);
  DESIGN_DECLARE_NAME(color_paragraph);
};


static constexpr auto introduction_paragraph
  = R"(Welcome to Design Lab! Design Lab is a desktop application
used to generate themes and assets for LVGL based programs
that run on both microcontrollers and desktop operating
systems including Windows, macOS, and Linux.

You will want to start with the Stratify Labs gui template project on github.
You can click the green button above to open Github in a browser.)";

static constexpr auto project_paragraph
  = R"(The first thing you need to do is configure your project. You need to decide
where your project will be located (directory) and where to generate code within
your project. You also need to specify the location of `lv_font_conv` so that Design Lab
can generate fonts.

The 'Project Directory' is the parent folder of your code. This is where the `designlab.json`
file will be stored which saves the Design Lab project settings.

The 'Source Directory' should be a source folder relative to 'Project Directory'. This
folder is where the output files will be generated.

You also need to specify your 'Path to node' and 'Path to lv_font_conv.js'. Design Lab
uses these values to invoke `lv_font_conv` for generating fonts and icons.
)";

static constexpr auto theme_paragraph
  = R"(Themes are defined using a series of JSON files that can be mixed and matched
if you need to create similar themes (like light and dark).

There is a main JSON file which references the JSON files you can use to define variables
for sizes, fonts, styles, and rules.

The helper files allow you to create variables. For example, in our sizes file we create
a variable called "$sizePadding" which is uses in the styles file to specify padding
of different objects.)";

static constexpr auto asset_paragraph
  = R"(You can specify any number of assets. Assets are regular files that are
integrated with the executable output file. These files can be accessed from
within the program to draw image files, load settings, and such.

You can include assets that are executable programs but they must be copied
to the filesystem before they can be invoked.)";

static constexpr auto font_paragraph
  = R"(You can specify what fonts you want to generate and what point sizes. You can
select whether the font should embed the selected icons.)";

static constexpr auto icon_paragraph
  = R"(The free FontAwesome 5 icons are included with Design Lab. You can choose
which icons you want to embed in your fonts (if the font has opted to embed icons).)";

static constexpr auto component_paragraph
  = R"(You can build components in real-time and then export source files that
can draw the components. Currently, you can only export components to C++ code
that use LvglAPI and DesignAPI libraries.)";

static constexpr auto color_paragraph
  = R"(The color tool is for previewing colors that can be used in the themes
you design.)";

var::String remove_newlines(const char *paragraph) {
  return String(paragraph)
    .replace(String::Replace().set_old_string("\n\n").set_new_string("__"))
    .replace(String::Replace().set_old_string("\n").set_new_string(" "))
    .replace(String::Replace().set_old_string("__").set_new_string("\n\n"));
}

}

GettingStarted::GettingStarted(const char *name) {
  construct_object(name);
  fill();

  clear_flag(Flags::scrollable)
    .add(Container(ViewObject::Names::content_container)
           .fill()
           .add(Column(LocalNames::main_column)));

  auto column = find<Column>(LocalNames::main_column);
  column.fill()
    .add(
      HeaderRow("Getting Started", icons::fa::github_brands, "gui", [](lv_event_t*){
        sys::System::launch_browser("https://github.com/StratifyLabs/gui");
      }))
    .add(HorizontalLine());

  {
    column.add(Heading2("Introduction"));
    static const auto text = remove_newlines(introduction_paragraph);
    column.add(Paragraph(LocalNames::introduction_paragraph, text.cstring()));
  }

  {
    column.add(Heading2("Project"));
    static const auto text = remove_newlines(project_paragraph);
    column.add(Paragraph(LocalNames::project_paragraph, text.cstring()));
  }

  {
    column.add(Heading2("Themes"));
    static const auto text = remove_newlines(theme_paragraph);
    column.add(Paragraph(LocalNames::theme_paragraph, text.cstring()));
  }

  {
    column.add(Heading2("Assets"));
    static const auto text = remove_newlines(asset_paragraph);
    column.add(Paragraph(LocalNames::asset_paragraph, text.cstring()));
  }

  {
    column.add(Heading2("Fonts"));
    static const auto text = remove_newlines(font_paragraph);
    column.add(Paragraph(LocalNames::font_paragraph, text.cstring()));
  }

  {
    column.add(Heading2("Icons"));
    static const auto text = remove_newlines(icon_paragraph);
    column.add(Paragraph(LocalNames::icon_paragraph, text.cstring()));
  }

  {
    column.add(Heading2("Components"));
    static const auto text = remove_newlines(component_paragraph);
    column.add(Paragraph(LocalNames::component_paragraph, text.cstring()));
  }

  {
    column.add(Heading2("Colors"));
    static const auto text = remove_newlines(color_paragraph);
    column.add(Paragraph(LocalNames::color_paragraph, text.cstring()));
  }
}
