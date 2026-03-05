#pragma once
#include <string>
#include <vector>

struct AppState {
  std::vector<std::string> databases = {"myapp_db", "laravel_db", "testing_db"};
  std::vector<std::string> tables = {"users", "posts", "migrations"};
  std::string query;
  int selected_db = 0;
  int selected_table = 0;
};
