#ifndef USERINFO_H
#define USERINFO_H
//个人信息抽象成为一个User类，个人信息包含ID，学号，姓名，性别，头像，学校。
#include <QObject>
#include <QPixmap>


class UserInfo
{

public:

public:
    UserInfo();
    UserInfo(const std::string studentID,const std::string name,const std::string gender,const std::string school);
    UserInfo(const std::string studentID);
    std::string toByteArray()const;//转换成id长度+id内容+name长度+name内容+gender长度+gender内容+school长度+school内容
public:
    std::string getStudentID()const {return m_studentID_;}
    std::string getName()const      {return m_name_;}
    std::string getGender()const    {return  m_gender_;}
    //QPixmap getHeadimg()const   {return m_headimg_;}
    std::string getSchool()const    {return m_school_;}
private:
    std::string m_studentID_;
    std::string m_name_;
    std::string m_gender_;
    //QPixmap m_headimg_;
    std::string m_school_;




};

#endif // USERINFO_H
