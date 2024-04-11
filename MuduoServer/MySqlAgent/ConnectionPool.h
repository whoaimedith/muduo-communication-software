#ifndef CONNECTIONPOOL_H
#define CONNECTIONPOOL_H

#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/connection.h>
#include <cppconn/exception.h>
#include <cppconn/driver.h>
#include <cppconn/statement.h>
#include <cppconn/resultset.h>
#include <cppconn/prepared_statement.h>
#include <queue>
#include <mutex>
#include <condition_variable>

class ConnectionPool {
private:
    std::queue<sql::Connection*> pool_;
    std::string host_;
    std::string user_;
    std::string password_;
    int max_connections_;
    std::mutex mutex_;
    std::condition_variable cv_;

public:
    ConnectionPool(const std::string& host, const std::string& user, const std::string& password, int max_connections)
        : host_(host), user_(user), password_(password), max_connections_(max_connections) {
        for (int i = 0; i < max_connections_; ++i) {
            sql::Connection* conn = createConnection();
            if (conn) {
                pool_.push(conn);
            }
        }
    }

    sql::Connection* getConnection() {
        std::unique_lock<std::mutex> lock(mutex_);
        while (pool_.empty()) {
            cv_.wait(lock);
        }
        sql::Connection* conn = pool_.front();
        pool_.pop();
        return conn;
    }

    void releaseConnection(sql::Connection* conn) {
        std::unique_lock<std::mutex> lock(mutex_);
        pool_.push(conn);
        cv_.notify_one();
    }

    ~ConnectionPool() {
        while (!pool_.empty()) {
            sql::Connection* conn = pool_.front();
            pool_.pop();
            delete conn;
        }
    }

private:
    sql::Connection* createConnection() {
        sql::mysql::MySQL_Driver* driver = sql::mysql::get_mysql_driver_instance();
        if (!driver) {
            return nullptr;
        }
        sql::Connection* conn = driver->connect(host_, user_, password_);
        if (!conn) {
            return nullptr;
        }
        return conn;
    }
};

#endif // CONNECTIONPOOL_H
