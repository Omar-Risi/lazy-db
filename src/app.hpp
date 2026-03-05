#pragma once
#include <string>
#include <vector>

struct AppState {
  std::vector<std::string> databases = {"myapp_db", "laravel_db", "testing_db"};
  std::vector<std::string> tables = {"users", "posts", "migrations"};
  std::string query;
  int selected_db = 0;
  int selected_table = 0;

  // Command mode
  bool command_mode = false;   // activated by ':'
  std::string command_input;   // text typed after ':'
  std::string command_result;  // feedback shown in command bar

  bool should_quit = false;    // set by :q
};
