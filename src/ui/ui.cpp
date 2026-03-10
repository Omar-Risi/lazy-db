#include "ui.hpp"

#include <ftxui/component/component.hpp>
#include <ftxui/component/component_options.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/dom/table.hpp>

using namespace ftxui;

Component BuildUI(App &state) {
  // Column 1 – database & table menus

  state.dbOption.on_enter = [&state] {
    state.UseDB(state.databases[state.selected_db]);
  };

  state.tableOption.on_enter = [&state] {
    state.UseTable(state.tables[state.selected_table]);
  };

  auto db_menu = Menu(&state.databases, &state.selected_db, state.dbOption);

  auto table_menu =
      Menu(&state.tables, &state.selected_table, state.tableOption);
  auto col1 = Container::Vertical({db_menu, table_menu});

  // Column 2 – query input
  auto query_input = Input(&state.query, "Enter query...");
  // Disable input focus when in command mode so ':' is not consumed
  auto query_input_maybe =
      Maybe(query_input, [&state] { return !state.command_mode; });
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

    Element helper = text("(:) open command mode") | color(Color::Yellow);
    Element results_view;
    if (state.columns.empty()) {
      results_view = text("No results found") | dim;
    } else {
      std::vector<std::vector<std::string>> rows;
      rows.push_back(state.columns);
      for (const auto &record : state.records) {
        rows.push_back(record);
      }

      Table table(rows);
      table.SelectAll().Border();
      table.SelectRow(0).Decorate(bold);
      table.SelectRow(0).SeparatorVertical();
      table.SelectRow(0).SeparatorHorizontal();

      if (rows.size() > 1) {
        table.SelectRows(1, static_cast<int>(rows.size())).SeparatorVertical();
      }

      results_view =
          vbox({
              text("Columns for " + state.table) | bold | color(Color::Yellow),
              separator(),
              table.Render() | xflex,
          }) |
          xflex;
    }

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
                results_view | border | flex,
            }) | flex,
        }) | border |
            flex,
        // Vim-style command bar
        cmd_bar | size(HEIGHT, EQUAL, 1),
        helper | size(HEIGHT, EQUAL, 1),
    });
  });

  return renderer;
}
