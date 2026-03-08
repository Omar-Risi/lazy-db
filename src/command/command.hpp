#pragma once
#include "../app.hpp"
#include <functional>
#include <map>
#include <string>
#include <vector>

using CommandFn =
    std::function<std::string(AppState &, std::vector<std::string>)>;

struct Command {
  std::string name;
  std::string description;
  CommandFn handler;
};

class CommandRegistry {
public:
  CommandRegistry(); // registers built-in commands

  void Register(const std::string &name, const std::string &description,
                CommandFn fn);
  std::string Execute(const std::string &raw, AppState &state);

const std::map<std::string, Command>& Commands() const { return commands_; }

private:
  std::map<std::string, Command> commands_;
};
