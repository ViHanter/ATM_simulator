#pragma once

#include <string>
#include <vector>

#include <libpq-fe.h>

class Database {
    public:
        struct Result {
            bool ok = false;
            std::vector<std::vector<std::string>> rows;
            std::string error;

            bool empty() const { return rows.empty(); }
            std::string scalar() const {
                return (rows.empty() || rows[0].empty()) ? std::string() : rows[0][0];
            }
        };

        Database();
        ~Database();

        Database(const Database&) = delete;
        Database& operator=(const Database&) = delete;

        bool ok() const { return conn_ != nullptr && PQstatus(conn_) == CONNECTION_OK; }
        const std::string& error() const { return error_; }

        Result exec(const std::string& sql);

        std::string quote(const std::string& value);

        bool ensureSchema();

    private:
        PGconn* conn_ = nullptr;
        std::string error_;
};
