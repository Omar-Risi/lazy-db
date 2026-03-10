#include "app.hpp"
#include <iostream>
#include <optional>

bool App::Connect(const std::string &connstr) {
  conn_ = PQconnectdb(connstr.c_str());
  return PQstatus(conn_) == CONNECTION_OK;
}

void App::Disconnect() {
  if (conn_) {
    if (PQstatus(conn_) != CONNECTION_OK)
      std::cerr << "Error: " << PQerrorMessage(conn_) << std::endl;
    PQfinish(conn_);
    conn_ = nullptr;
  }
}

std::vector<std::string> App::FetchDatabases() {
  std::vector<std::string> dbs;
  if (!IsConnected())
    return dbs;

  PGresult *res = PQexec(
      conn_, "SELECT datname FROM pg_database WHERE datistemplate = false;");
  if (PQresultStatus(res) == PGRES_TUPLES_OK) {
    for (int i = 0; i < PQntuples(res); i++)
      dbs.push_back(PQgetvalue(res, i, 0));
  }
  PQclear(res);

  return dbs;
}

std::vector<std::string> App::FetchTables() {
  std::vector<std::string> tbls;
  if (!IsConnected())
    return tbls;

  PGresult *res = PQexec(conn_, "SELECT tablename FROM pg_tables "
                                "WHERE schemaname = 'public' "
                                "ORDER BY tablename;");
  if (PQresultStatus(res) == PGRES_TUPLES_OK) {
    for (int i = 0; i < PQntuples(res); i++)
      tbls.push_back(PQgetvalue(res, i, 0));
  }
  PQclear(res);
  return tbls;
}

bool App::UseDB(std::optional<std::string> dbname) {
  if (!dbname)
    return false;

  db = *dbname;

  // Reconnect to the selected database
  Disconnect();
  std::string connstr =
      "host=localhost user=laravel password=password port=5432 dbname=" + db;
  if (!Connect(connstr))
    return false;

  tables = FetchTables();
  return true;
}
