#include "registerdialog.h"
#include "ui_registerdialog.h"
#include <QMessageBox>
RegisterDialog::RegisterDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RegisterDialog),
    m_nCondition(m_nMutex)
{
    ui->setupUi(this);
}

bool RegisterDialog::isStringAllDigits(const string &str)
{
    {
            for (char c : str) {
                if (!std::isdigit(c)) {
                    return false;
                }
            }
            return true;
        }
}

RegisterDialog::~RegisterDialog()
{
    delete ui;
}

void RegisterDialog::on_RegisterButton_clicked()
{    std::string _strUserId = ui->userIdEdit->text().toStdString();
     std::string _strPassword = ui->passwordEdit->text().toStdString();
     std::string _confirmEdit=ui->lineEdit_4->text().toStdString();
     if(_strUserId=="")
     {
         {
             // 创建一个信息对话框
                QMessageBox messageBox;
                messageBox.setWindowTitle("提示");
                messageBox.setText("学号不能为空，请重新输入");
                messageBox.setIcon(QMessageBox::Information);

                // 添加一个 OK 按钮
                messageBox.addButton(QMessageBox::Ok);

                // 显示消息对话框
                messageBox.exec();
                ui->passwordEdit->clear();
                ui->lineEdit_4->clear();

         }
     }

     else if(!isStringAllDigits(_strUserId))
     {
         // 创建一个信息对话框
            QMessageBox messageBox;
            messageBox.setWindowTitle("提示");
            messageBox.setText("学号必须全是数字，请重新输入");
            messageBox.setIcon(QMessageBox::Information);

            // 添加一个 OK 按钮
            messageBox.addButton(QMessageBox::Ok);

            // 显示消息对话框
            messageBox.exec();
            ui->passwordEdit->clear();
            ui->lineEdit_4->clear();
            ui->userIdEdit->clear();

     }
     else if(_strUserId.length()!=8)
     {
         // 创建一个信息对话框
            QMessageBox messageBox;
            messageBox.setWindowTitle("提示");
            messageBox.setText("学号必须为8位数字，请重新输入");
            messageBox.setIcon(QMessageBox::Information);

            // 添加一个 OK 按钮
            messageBox.addButton(QMessageBox::Ok);

            // 显示消息对话框
            messageBox.exec();
            ui->passwordEdit->clear();
            ui->lineEdit_4->clear();
            ui->userIdEdit->clear();

     }
     else if(_strPassword=="" || _confirmEdit=="")
     {
         // 创建一个信息对话框
            QMessageBox messageBox;
            messageBox.setWindowTitle("提示");
            messageBox.setText("密码或确认密码为空，请重新输入");
            messageBox.setIcon(QMessageBox::Information);

            // 添加一个 OK 按钮
            messageBox.addButton(QMessageBox::Ok);

            // 显示消息对话框
            messageBox.exec();
            ui->passwordEdit->clear();
            ui->lineEdit_4->clear();
            ui->userIdEdit->clear();
     }
     else if(_confirmEdit!=_strPassword)
     {
         // 创建一个信息对话框
            QMessageBox messageBox;
            messageBox.setWindowTitle("提示");
            messageBox.setText("两次输入密码不一致，请重新输入");
            messageBox.setIcon(QMessageBox::Information);

            // 添加一个 OK 按钮
            messageBox.addButton(QMessageBox::Ok);

            // 显示消息对话框
            messageBox.exec();
            ui->passwordEdit->clear();
            ui->lineEdit_4->clear();

     }
     else{

   MuduoClient* _pClient = MuduoClient::instance(nullptr, InetAddress());
    assert(_pClient);
    // set Register callback to process the register result
    // set Error callback to process error
    // not concern the timeout callback
    _pClient->setRegisterCallback(
        std::bind(&RegisterDialog::RegisterCallback, this, std::placeholders::_1));
    _pClient->setErrorCallback(
        std::bind(&RegisterDialog::ErrorCallback, this, std::placeholders::_1));

    // pack and send to server
    // information format
    // 4 byte for length, thus we can send a message at most 2^32-1 byte
    // 2 byte for type, thus we can process 2^16-1 different types.
    // for register type
    // the content format
    // 1 byte for user-id length, thus we can process a user-id at most 2^8-1 byte
    // 1 byte for password length, thus we can process a password at most 2^8-1 byte
    bool _bRet = _pClient->sendRegisterMessage(_strUserId, _strPassword);
    assert(_bRet);


    // use mutex and condition variable realize the wait and sync between two threads
    // at here
    // 1.we lock+wait condition
    // 2.when the condition is signaled,we continue to process the result
    // 3.when we finish process,release lock
    m_nMutex.lock();
    m_nRegisterRet = -1;
    m_bError = false;
    while (m_nRegisterRet == -1
        && m_bError == false)
    {
      m_nCondition.wait();
    }

    int _nRegisterRet = m_nRegisterRet;
    bool _bError = m_bError;
    m_nMutex.unlock();
    if(_bError)
    {
        // 创建一个信息对话框
           QMessageBox messageBox;
           messageBox.setWindowTitle("提示");
           messageBox.setText("注册发生未知错误，请稍后注册");
           messageBox.setIcon(QMessageBox::Information);

           // 添加一个 OK 按钮
           messageBox.addButton(QMessageBox::Ok);

           // 显示消息对话框
           messageBox.exec();
           ui->passwordEdit->clear();
           ui->lineEdit_4->clear();
           ui->userIdEdit->clear();
    }
    else
    {
        if(_nRegisterRet == 0)
        {
            // 创建一个信息对话框
               QMessageBox messageBox;
               messageBox.setWindowTitle("提示");
               messageBox.setText("注册成功，请登陆");
               messageBox.setIcon(QMessageBox::Information);

               // 添加一个 OK 按钮
               messageBox.addButton(QMessageBox::Ok);

               // 显示消息对话框
               messageBox.exec();
               this->close();
        }
        else if(_nRegisterRet == 1)
        {
            // 创建一个信息对话框
               QMessageBox messageBox;
               messageBox.setWindowTitle("提示");
               messageBox.setText("用户已经注册过，无法再次注册");
               messageBox.setIcon(QMessageBox::Information);

               // 添加一个 OK 按钮
               messageBox.addButton(QMessageBox::Ok);

               // 显示消息对话框
               messageBox.exec();
               ui->passwordEdit->clear();
               ui->lineEdit_4->clear();
               ui->userIdEdit->clear();
        }
        else
        {
            // 创建一个信息对话框
               QMessageBox messageBox;
               messageBox.setWindowTitle("提示");
               messageBox.setText("注册异常，请稍候重试");
               messageBox.setIcon(QMessageBox::Information);

               // 添加一个 OK 按钮
               messageBox.addButton(QMessageBox::Ok);

               // 显示消息对话框
               messageBox.exec();
               ui->passwordEdit->clear();
               ui->lineEdit_4->clear();
               ui->userIdEdit->clear();
        }
    }


}
}
void RegisterDialog::RegisterCallback(int nType_)
{
    // at here
    // we lock
    // change variable
    // signal condition
    // release lock
    MutexLockGuard lock(m_nMutex);
    m_nRegisterRet = nType_;
    m_nCondition.notify();
}

void RegisterDialog::ErrorCallback(const TcpConnectionPtr&)
{
    // Err
    // at here
    // we lock
    // change variable
    // signal condition
    // release lock
    MutexLockGuard lock(m_nMutex);
    m_bError = true;
    m_nCondition.notify();
}

