#include "mysqlconnector.h"



MySQLConnector::MySQLConnector(const string &host, const string &user, const string &password, const string &database)
        : host_(host), user_(user), password_(password), database_(database)
{
    try {
        driver_ = sql::mysql::get_mysql_driver_instance();
        con_ = driver_->connect(host_, user_, password_);
        con_->setSchema(database_);
    } catch (sql::SQLException &e) {
        // Handle any errors
        LOG_FATAL<<e.what();
    }
}



bool MySQLConnector::executeQuery(const string &query)
{
    try {
        sql::Statement *stmt;
        sql::ResultSet *res;
        stmt = con_->createStatement();
        res = stmt->executeQuery(query);

        // Process the results or do something with them...

        delete res;
        delete stmt;
        return true;
    } catch (sql::SQLException &e) {
        // Handle any errors
        return false;

    }
}

void MySQLConnector::setSchema(const string &database)
{
    try {
        con_->setSchema(database);

    }  catch (sql::SQLException e) {
        LOG_FATAL<<e.what();
    }

}
