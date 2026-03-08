#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>

#include "app.hpp"
#include "command/command.hpp"
#include "ui/ui.hpp"

using namespace ftxui;

int main() {
  auto screen = ScreenInteractive::Fullscreen();

  AppState state;
  CommandRegistry registry;
  // RegisterBuiltinCommands(registry);

  auto ui = BuildUI(state);

  auto with_keys = CatchEvent(ui, [&](Event event) {
    // Enter command mode
    if (!state.command_mode && event == Event::Character(':')) {
      state.command_mode = true;
      state.command_input.clear();
      return true;
    }

    if (state.command_mode) {
      // Exit command mode
      if (event == Event::Escape) {
        state.command_mode = false;
        state.command_input.clear();
        state.command_result.clear();
        return true;
      }
      // Execute command
      if (event == Event::Return) {
        state.command_result = registry.Execute(state.command_input, state);
        state.command_mode = false;
        state.command_input.clear();
        if (state.should_quit)
          screen.ExitLoopClosure()();
        return true;
      }
      // Backspace
      if (event == Event::Backspace) {
        if (!state.command_input.empty())
          state.command_input.pop_back();
        return true;
      }
      // Type characters into command input
      if (event.is_character()) {
        state.command_input += event.character();
        return true;
      }
    }

    return false;
  });

  screen.Loop(with_keys);
  return 0;
}
