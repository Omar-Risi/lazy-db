#include "ui.hpp"

#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>

using namespace ftxui;

Component BuildUI(AppState &state) {
  // Column 1 – database & table menus
  auto db_menu = Menu(&state.databases, &state.selected_db);
  auto table_menu = Menu(&state.tables, &state.selected_table);
  auto col1 = Container::Vertical({db_menu, table_menu});

  // Column 2 – query input
  auto query_input = Input(&state.query, "Enter query...");
  // Disable input focus when in command mode so ':' is not consumed
  auto query_input_maybe = Maybe(query_input, [&state] { return !state.command_mode; });
  auto col2 = Container::Vertical({query_input_maybe});

  // Combined layout
  auto layout = Container::Horizontal({col1, col2});

  auto renderer = Renderer(layout, [=, &state] {
    // Command bar: shows ":cmd" while typing, or last result
    Element cmd_bar;
    if (state.command_mode)
      cmd_bar = text(":" + state.command_input) | color(Color::White);
    else if (!state.command_result.empty())
      cmd_bar = text(state.command_result) | color(Color::Yellow);
    else
      cmd_bar = text("-- NORMAL --") | dim;

    return vbox({
               hbox({
                   // Column 1
                   vbox({
                       text("Databases") | bold | color(Color::Blue),
                       separator(),
                       db_menu->Render() | border | flex,
                       text("Tables") | bold | color(Color::Green),
                       separator(),
                       table_menu->Render() | border | flex,
                   }) | size(WIDTH, EQUAL, 30),
                   separator(),
                   // Column 2
                   vbox({
                       text("Query") | bold | color(Color::Yellow),
                       separator(),
                       query_input->Render() | border,
                       separator(),
                       text("Results") | bold | color(Color::Yellow),
                       separator(),
                       text("No results yet") | dim | border | flex,
                   }) | flex,
               }) | border | flex,
               // Vim-style command bar
               cmd_bar | size(HEIGHT, EQUAL, 1),
           });
  });

  return renderer;
}
