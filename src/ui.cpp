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
  auto col2 = Container::Vertical({query_input});

  // Combined layout
  auto layout = Container::Horizontal({col1, col2});

  auto renderer = Renderer(layout, [=, &state] {
    return hbox({
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
           }) |
           border;
  });

  return renderer;
}
