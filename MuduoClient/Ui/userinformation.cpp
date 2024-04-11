#include "userinformation.h"
#include "ui_userinformation.h"
#include "userinfo.h"
#include "userinformation.h"
#include "MuduoApp/muduoclient.h"

UserInformation::UserInformation(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UserInformation),
    m_Condition(m_MutexForUpdateMyInfo),
    m_bError(false),
    m_bRet(false)


{
    ui->setupUi(this);
    ui->saveButton->setChecked(true);
    ui->name->setReadOnly(true);
    ui->gender->setEnabled(false);
    ui->lineEdit_5->setReadOnly(true);


    //this->setWindowModality(Qt::WindowModal);
    MuduoClient* _pClient = MuduoClient::instance(nullptr, InetAddress());
         _pClient->setUpdateMyInfoCallback(std::bind(&UserInformation::UpdateMyInfoCallback,this,_1));
         _pClient->setModifyMyInfoCallback(std::bind(&UserInformation::ModifyMyInfoCallback,this,_1));
}
void UserInformation::UpdateMyInfo()
{     MuduoClient* _pClient = MuduoClient::instance(nullptr, InetAddress());
      std::cout<<"准备发送信息 我的id是"<<m_userID<<std::endl<<std::flush;
      _pClient->sendUpdateMyInfoMessage(m_userID);
          std::cout<<"信息发送成功，准备上锁"<<std::endl<<std::flush;
        m_MutexForUpdateMyInfo.lock();
          std::cout<<"成功上锁"<<std::endl<<std::flush;
        while(m_bError==false && m_bRet==false)
        {
             std::cout<<"前端等待，释放锁"<<std::endl<<std::flush;
            m_Condition.wait();

        }
       bool m_Error=m_bError;

       m_MutexForUpdateMyInfo.unlock();
         std::cout<<"执行到解锁"<<std::endl<<std::flush;
         if(m_Error)
         {
             assert(false);
         }
         else
         {
             ui->label_2->setText(QString::fromStdString(m_userInfo.getStudentID()));
             ui->name->setText(QString::fromStdString(m_userInfo.getName()));
             std::cout<<"执行到设置姓名"<<std::endl<<std::flush;
             if(m_userInfo.getGender()=="男")
            {

                 ui->gender->setCurrentIndex(0);
            }else if(m_userInfo.getGender()=="女")
            {
                 ui->gender->setCurrentIndex(1);

             }
             else
            {
                 ui->gender->setCurrentIndex(2);
             }
             //ui->toolButton->setIcon(QIcon(m_userInfo.getHeadimg()));
             ui->lineEdit_5->setText(QString::fromStdString(m_userInfo.getSchool()));


         }

}

void UserInformation::ModifyMyInfo(UserInfo newInfo)
{
    MuduoClient* _pClient = MuduoClient::instance(nullptr, InetAddress());

          _pClient->sendModifyMyInfoMessage(newInfo);

            m_MutexForUpdateMyInfo.lock();
             while(m_bError==false && m_bRet==false)
             {

                 m_Condition.wait();

             }
            bool m_Error=m_bError;

            m_MutexForUpdateMyInfo.unlock();

              if(m_Error)
              {
                  assert(false);
              }
              else
              {
                  UpdateMyInfo();
              }

}



UserInformation::~UserInformation()
{
    delete ui;
}

void UserInformation::UpdateMyInfoCallback(UserInfo latestMyInfo)
{
    MutexLockGuard m_lock(m_MutexForUpdateMyInfo);
    m_userInfo=latestMyInfo;
    m_bError=false;
    m_bRet=true;
    m_Condition.notify();
}

void UserInformation::ModifyMyInfoCallback(int ret)
{
    MutexLockGuard m_lock(m_MutexForUpdateMyInfo);
   if(ret==1)
   {

    m_bError=false;
    m_bRet=true;
   }
   else
    {
       m_bError=true;
       m_bRet=false;
   }

    m_Condition.notify();
}




void UserInformation::on_saveButton_clicked()
{
    ui->name->setReadOnly(false);
    ui->gender->setEnabled(true);
    ui->lineEdit_5->setReadOnly(false);
    /*ui->label_2->setText(ui->label_2->text());
    ui->name->setText(ui->name->text());
    ui->gender->setCurrentIndex(ui->gender->currentIndex());
    ui->lineEdit_5->setText(ui->lineEdit_5->text())*/;
    UserInfo newInfo(ui->label_2->text().toStdString(),ui->name->text().toStdString(),ui->gender->currentText().toStdString(),ui->lineEdit_5->text().toStdString());
    ModifyMyInfo(newInfo);
    ui->name->setReadOnly(true);
    ui->gender->setEnabled(false);
    ui->lineEdit_5->setReadOnly(true);


}


void UserInformation::on_modifyButton_clicked()
{
    ui->name->setReadOnly(false);
    ui->gender->setEnabled(true);
    ui->lineEdit_5->setReadOnly(false);

}

