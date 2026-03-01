#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
using namespace std;
using namespace ftxui;

int main() {
  auto screen = ScreenInteractive::Fullscreen();

  // placeholder data
  vector<string> databases = {"myapp_db", "laravel_db", "testing_db"};
  vector<string> tables = {"users", "posts", "migrations"};
  string query = "";
  int selected_db = 0;
  int selected_table = 0;

  // col 1 components
  auto db_menu = Menu(&databases, &selected_db);
  auto table_menu = Menu(&tables, &selected_table);
  auto col1 = Container::Vertical({db_menu, table_menu});

  // col 2 components
  auto query_input = Input(&query, "Enter query...");
  auto col2 = Container::Vertical({query_input});

  // combine cols
  auto layout = Container::Horizontal({col1, col2});

  auto renderer = Renderer(layout, [&] {
    return hbox({
               // col 1
               vbox({
                   text("Databases") | bold | color(Color::Blue),
                   separator(),
                   db_menu->Render() | border | flex,
                   text("Tables") | bold | color(Color::Green),
                   separator(),
                   table_menu->Render() | border | flex,
               }) | size(WIDTH, EQUAL, 30),

               separator(),

               // col 2
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

  auto with_keys = CatchEvent(renderer, [&](Event event) {
    if (event == Event::Character('q')) {
      screen.ExitLoopClosure()();
      return true;
    }
    return false;
  });

  screen.Loop(with_keys);
  return 0;
}
