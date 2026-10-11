#include "db.h"

#include <cstdlib>

namespace {

const char* envOr(const char* name, const char* fallback) {
    const char* value = std::getenv(name);
    return (value && *value) ? value : fallback;
}

} // namespace

Database::Database() {
    const char* host     = envOr("ATM_DB_HOST", "127.0.0.1");
    const char* port     = envOr("ATM_DB_PORT", "5432");
    const char* dbname   = envOr("ATM_DB_NAME", "atm_db");
    const char* user     = envOr("ATM_DB_USER", "atm");
    const char* password = envOr("ATM_DB_PASSWORD", "atm");

    conn_ = PQsetdbLogin(host, port, nullptr, nullptr, dbname, user, password);
    if (!ok()) {
        error_ = PQerrorMessage(conn_);
    }
}

Database::~Database() {
    if (conn_) {
        PQfinish(conn_);
        conn_ = nullptr;
    }
}

Database::Result Database::exec(const std::string& sql) {
    Result result;
    if (!conn_) {
        result.error = "Нет соединения с БД.";
        return result;
    }

    PGresult* res = PQexec(conn_, sql.c_str());
    if (!res) {
        result.error = PQerrorMessage(conn_);
        return result;
    }

    ExecStatusType status = PQresultStatus(res);
    if (status != PGRES_TUPLES_OK && status != PGRES_COMMAND_OK) {
        result.error = PQresultErrorMessage(res);
        PQclear(res);
        return result;
    }

    int rows = PQntuples(res);
    int cols = PQnfields(res);
    result.rows.reserve(rows);
    for (int r = 0; r < rows; ++r) {
        std::vector<std::string> row;
        row.reserve(cols);
        for (int c = 0; c < cols; ++c) {
            if (PQgetisnull(res, r, c)) row.emplace_back();
            else row.emplace_back(PQgetvalue(res, r, c));
        }
        result.rows.push_back(std::move(row));
    }

    result.ok = true;
    PQclear(res);
    return result;
}

std::string Database::quote(const std::string& value) {
    if (!conn_) return "'" + value + "'";

    char* escaped = PQescapeLiteral(conn_, value.c_str(), value.size());
    if (!escaped) {
        error_ = PQerrorMessage(conn_);
        return "''";
    }
    std::string result(escaped);
    PQfreemem(escaped);
    return result;
}

bool Database::ensureSchema() {
    static const char* kSchema =
        "CREATE TABLE IF NOT EXISTS accounts ("
        "  id       SERIAL PRIMARY KEY,"
        "  name     TEXT NOT NULL UNIQUE,"
        "  password TEXT NOT NULL"
        ");"
        "CREATE TABLE IF NOT EXISTS bank ("
        "  id      SERIAL PRIMARY KEY,"
        "  name    TEXT NOT NULL UNIQUE,"
        "  balance BIGINT NOT NULL DEFAULT 0"
        ");"
        "CREATE TABLE IF NOT EXISTS cards ("
        "  id         SERIAL PRIMARY KEY,"
        "  account_id INTEGER NOT NULL REFERENCES accounts(id) ON DELETE CASCADE,"
        "  card_num   TEXT NOT NULL UNIQUE,"
        "  cvv        TEXT NOT NULL,"
        "  data       TEXT NOT NULL,"
        "  bank       TEXT REFERENCES bank(name),"
        "  pin_code   TEXT NOT NULL DEFAULT '',"
        "  balance    BIGINT NOT NULL DEFAULT 0"
        ");"
        "CREATE TABLE IF NOT EXISTS money_bag ("
        "  account_id INTEGER NOT NULL REFERENCES accounts(id) ON DELETE CASCADE,"
        "  nominal    INTEGER NOT NULL,"
        "  count      INTEGER NOT NULL DEFAULT 0,"
        "  PRIMARY KEY (account_id, nominal)"
        ");"
        "CREATE TABLE IF NOT EXISTS transactions ("
        "  id         SERIAL PRIMARY KEY,"
        "  account_id INTEGER NOT NULL REFERENCES accounts(id) ON DELETE CASCADE,"
        "  card_num   TEXT,"
        "  type       TEXT NOT NULL,"
        "  amount     BIGINT NOT NULL,"
        "  detail     TEXT NOT NULL DEFAULT '',"
        "  created_at TIMESTAMPTZ NOT NULL DEFAULT now()"
        ");"
        "CREATE TABLE IF NOT EXISTS atm_money_bag ("
        "  nominal INTEGER PRIMARY KEY,"
        "  count   INTEGER NOT NULL DEFAULT 0"
        ");"
        "INSERT INTO bank(name) VALUES ('RofloBank'),('SbeerBank'),('Tankoff'),('MusorBank')"
        " ON CONFLICT (name) DO NOTHING;"
        "INSERT INTO atm_money_bag(nominal, count) VALUES"
        " (50,500),(100,500),(200,500),(500,500),(1000,500),(2000,500),(5000,500)"
        " ON CONFLICT (nominal) DO NOTHING;";

    Result result = exec(kSchema);
    if (!result.ok) {
        error_ = result.error;
        return false;
    }
    return true;
}
