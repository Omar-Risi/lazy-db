#include "command.hpp"
#include <algorithm>
#include <sstream>

// ---------------------------------------------------------------------------
// Helpers
// ---------------------------------------------------------------------------

static std::vector<std::string> ParseArgs(const std::string &input) {
  std::istringstream iss(input);
  std::vector<std::string> tokens;
  std::string tok;
  while (iss >> tok)
    tokens.push_back(tok);
  return tokens;
}

// ---------------------------------------------------------------------------
// Built-in command registrations
// ---------------------------------------------------------------------------

CommandRegistry::CommandRegistry() {

  Register("q", "Quit the application",
           [](AppState &state, std::vector<std::string>) -> std::string {
             state.should_quit = true;
             return "";
           });

  Register("use", "Switch database: use <db>",
           [](AppState &state, std::vector<std::string> args) -> std::string {
             if (args.empty())
               return "Usage: use <database>";
             auto it = std::find(state.databases.begin(), state.databases.end(),
                                 args[0]);
             if (it == state.databases.end())
               return "Database '" + args[0] + "' not found";
             state.selected_db =
                 static_cast<int>(std::distance(state.databases.begin(), it));
             return "Switched to " + args[0];
           });

  Register("clear", "Clear the query input",
           [](AppState &state, std::vector<std::string>) -> std::string {
             state.query.clear();
             return "Query cleared";
           });

  Register("help", "List available commands",
           [](AppState &state, std::vector<std::string>) -> std::string {
             std::string out = "Commands:";
             // help text is built from the registry in ui.cpp; this is the
             // fallback result string shown in the command bar.
             (void)state;
             return out + " q, use <db>, clear, help";
           });
}

// ---------------------------------------------------------------------------
// Public interface
// ---------------------------------------------------------------------------

void CommandRegistry::Register(const std::string &name,
                               const std::string &description, CommandFn fn) {
  commands_[name] = Command{name, description, std::move(fn)};
}

std::string CommandRegistry::Execute(const std::string &raw, AppState &state) {
  auto tokens = ParseArgs(raw);
  if (tokens.empty())
    return "";

  auto it = commands_.find(tokens[0]);
  if (it == commands_.end())
    return "Unknown command: " + tokens[0];

  std::vector<std::string> args(tokens.begin() + 1, tokens.end());
  return it->second.handler(state, args);
}
