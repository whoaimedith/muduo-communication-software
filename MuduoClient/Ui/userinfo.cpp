#include "userinfo.h"

//个人信息抽象成为一个User类，个人信息包含ID，学号，姓名，性别，头像，学校。


UserInfo::UserInfo()
    :m_studentID_("未知"),
     m_name_("未知"),
     m_gender_("未知"),
     //m_headimg_(QPixmap(":/image/Resouces/Customer Copy.png")),
     m_school_("未知")
{

}
UserInfo::UserInfo(const std::string studentID)
            :m_studentID_(studentID),
             m_name_("未知"),
             m_gender_("未知"),
             //m_headimg_(QPixmap(":/image/Resouces/Customer Copy.png")),
             m_school_("未知")

{

}
//String m_studentID_;
//std::string m_name_;
//std::string m_gender_;
//QPixmap m_headimg_;
//std::string m_school_;
std::string UserInfo::toByteArray() const
{
    std::string userInfo;
    int8_t studentIdLen=m_studentID_.length();
    userInfo.append((char*)&studentIdLen,sizeof studentIdLen);
    userInfo.append(m_studentID_);

    int8_t nameLen=m_name_.length();
    userInfo.append((char*)&nameLen,sizeof nameLen);
    userInfo.append(m_name_);

    int8_t genderLen=m_gender_.length();
    userInfo.append((char*)&genderLen,sizeof genderLen);
    userInfo.append(m_gender_);

    int8_t schoolLen=m_school_.length();
    userInfo.append((char*)&schoolLen,sizeof schoolLen);
    userInfo.append(m_school_);
    return userInfo;

}



UserInfo::UserInfo(const std::string studentID, const std::string name, const std::string gender,const  std::string school)
            :m_studentID_(studentID),
             m_name_(name),
             m_gender_(gender),
            // m_headimg_(headimg),
             m_school_(school)
{

}
