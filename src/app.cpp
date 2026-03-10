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

std::vector<std::string> App::FetchColumns() {
  std::vector<std::string> cols;
  if (!IsConnected() || table.empty())
    return cols;

  const char *params[] = {table.c_str()};
  PGresult *res =
      PQexecParams(conn_,
                   "SELECT column_name "
                   "FROM information_schema.columns "
                   "WHERE table_schema = 'public' AND table_name = $1 "
                   "ORDER BY ordinal_position;",
                   1, nullptr, params, nullptr, nullptr, 0);

  if (PQresultStatus(res) == PGRES_TUPLES_OK) {
    for (int i = 0; i < PQntuples(res); i++)
      cols.push_back(PQgetvalue(res, i, 0));
  }

  PQclear(res);
  return cols;
}

std::vector<std::vector<std::string>> App::FetchRecords() {
  std::vector<std::vector<std::string>> rows;
  if (!IsConnected() || table.empty())
    return rows;

  std::string query = "SELECT * FROM \"" + table + "\" LIMIT 50;";
  PGresult *res = PQexec(conn_, query.c_str());

  if (PQresultStatus(res) == PGRES_TUPLES_OK) {
    const int row_count = PQntuples(res);
    const int col_count = PQnfields(res);

    for (int i = 0; i < row_count; ++i) {
      std::vector<std::string> row;
      row.reserve(col_count);
      for (int j = 0; j < col_count; ++j) {
        if (PQgetisnull(res, i, j))
          row.push_back("NULL");
        else
          row.push_back(PQgetvalue(res, i, j));
      }
      rows.push_back(std::move(row));
    }
  }

  PQclear(res);
  return rows;
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

bool App::UseTable(std::optional<std::string> tableName) {
  if (!tableName)
    return false;

  table = *tableName;

  columns = FetchColumns();
  records = FetchRecords();

  return true;
}
