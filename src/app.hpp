#pragma once
#include <ftxui/component/component_options.hpp>
#include <libpq-fe.h>
#include <optional>
#include <string>
#include <vector>

class App {
public:
  App() = default;
  ~App() { Disconnect(); }

  bool Connect(const std::string &connstr);
  void Disconnect();
  bool IsConnected() const { return conn_ != nullptr; }
  bool UseDB(std::optional<std::string> dbname = std::nullopt);
  bool UseTable(std::optional<std::string> tableName = std::nullopt);

  std::vector<std::string> FetchDatabases();
  std::vector<std::string> FetchTables();
  std::vector<std::string> FetchColumns();

  // UI state
  std::vector<std::string> databases;
  std::string db;
  std::string table;
  std::vector<std::string> tables;
  std::vector<std::string> columns;
  std::string query;
  int selected_db = 0;
  int selected_table = 0;
  ftxui::MenuOption dbOption;
  ftxui::MenuOption tableOption;

  bool command_mode = false;
  std::string command_input;
  std::string command_result;
  bool should_quit = false;

private:
  PGconn *conn_ = nullptr;
};
