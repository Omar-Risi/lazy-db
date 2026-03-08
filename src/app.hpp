#pragma once
#include <string>
#include <vector>
#include <libpq-fe.h>

class App {
public:
  App() = default;
  ~App() { Disconnect(); }

  bool Connect(const std::string& connstr);
  void Disconnect();
  bool IsConnected() const { return conn_ != nullptr; }

  std::vector<std::string> FetchDatabases();

  // UI state
  std::vector<std::string> databases;
  std::vector<std::string> tables;
  std::string query;
  int selected_db = 0;
  int selected_table = 0;

  bool command_mode = false;
  std::string command_input;
  std::string command_result;
  bool should_quit = false;

private:
  PGconn* conn_ = nullptr;
};
