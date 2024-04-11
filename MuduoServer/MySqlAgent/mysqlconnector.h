#ifndef MYSQLCONNECTOR_H
#define MYSQLCONNECTOR_H

#include <QObject>

#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/statement.h>
#include <cppconn/resultset.h>
#include <cppconn/exception.h>
#include <string>
#include "MuduoApp/lib.h"
 //  执行不返回结果集的查询 查询准备 绑定参数 执行查询保存到结果集 结果集行数 结果集跳到某一行 结果集的字段有哪些 获取结果集的一行内容 查询这一行的第几个字段 修改这一行的某一个字段 释放结果集
class MySQLConnector {
public:
    MySQLConnector(const std::string& host, const std::string& user, const std::string& password, const std::string& database);

    MySQLConnector(MySQLConnector&)=delete;

    ~MySQLConnector() {
        delete con_;
    }


    void setSchema(const std::string & database);//选择数据库
    std::string getSchema()const{return con_->getSchema();}//查看当前的数据库
     bool executeQuery(const std::string& query) ;






private:
    sql::mysql::MySQL_Driver *driver_;
    sql::Connection *con_;
    std::string host_;
    std::string user_;
    std::string password_;
    std::string database_;
};



#endif // MYSQLCONNECTOR_H
