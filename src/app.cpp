#include "app.hpp"
#include <iostream>
#include <stdexcept>

bool App::Connect(const std::string &connstr) {
  conn_ = PQconnectdb(connstr.c_str());
  return PQstatus(conn_) == CONNECTION_OK;
}

void App::Disconnect() {
  if (conn_) {
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
