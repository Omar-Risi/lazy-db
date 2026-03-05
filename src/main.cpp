#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>

#include "app.hpp"
#include "ui.hpp"

using namespace ftxui;

int main() {
  auto screen = ScreenInteractive::Fullscreen();

  AppState state;
  auto ui = BuildUI(state);

  auto with_keys = CatchEvent(ui, [&](Event event) {
    if (event == Event::Character('q')) {
      screen.ExitLoopClosure()();
      return true;
    }
    return false;
  });

  screen.Loop(with_keys);
  return 0;
}
