#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>

using namespace std;
using namespace ftxui;

int main() {
  auto screen = ScreenInteractive::Fullscreen();

  // placeholder database list
  vector<string> databases = {
      "myapp_db",
      "laravel_db",
      "testing_db",
  };

  int selected = 0;

  auto menu = Menu(&databases, &selected);

  auto renderer = Renderer(menu, [&] {
    return vbox({
               text("LazyDB") | bold | color(Color::Blue),
               separator(),
               text("Databases") | dim,
               menu->Render() | border,
               separator(),
               text("[c] Create  [d] Drop  [q] Quit") | dim,
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
