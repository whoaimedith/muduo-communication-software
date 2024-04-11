#ifndef USERINFORMATION_H
#define USERINFORMATION_H
#include "../MyMuduo/Lib/lib.h"
#include <QWidget>
#include "userinfo.h"

namespace Ui {
class UserInformation;
}

class UserInformation : public QWidget
{
    Q_OBJECT

public:
    explicit UserInformation(QWidget *parent = nullptr);
    void setUserID(std::string ID){m_userID=ID;}
    void UpdateMyInfo();
    void ModifyMyInfo(UserInfo newInfo);
    void UpdateMyInfoCallback(UserInfo latestMyInfo);
    void ModifyMyInfoCallback(int ret);

    std::string getMyId()const {return m_userID;}
    std::string getMyName()const{return m_name.toStdString();}
    ~UserInformation();

protected:
    void closeEvent(QCloseEvent *event) override
    {
        Q_UNUSED(event);
        this->hide();


    }



private slots:
    void on_saveButton_clicked();

    void on_modifyButton_clicked();

private:
    Ui::UserInformation *ui;
    MutexLock m_MutexForUpdateMyInfo;
    Condition m_Condition;
    bool      m_bError;
    bool      m_bRet;
    UserInfo m_userInfo;
    std::string m_userID;
    QString m_name;
    QString m_gender;
    QPixmap headimg;
    QString m_school;
};

#endif // USERINFORMATION_H
