

#include "addfrienddialog.h"
#include "ui_addfrienddialog.h"

AddFriendDialog::AddFriendDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddFriendDialog),
    m_nCondition(m_nMutex)
{
    ui->setupUi(this);
    Initialize();
}

void AddFriendDialog::Initialize()
{
    connect(ui->addBtn, SIGNAL(clicked(bool)), this, SLOT(AddClicked(bool)));
    connect(ui->exitBtn, SIGNAL(clicked(bool)), this, SLOT(ExitClicked(bool)));
}

void AddFriendDialog::closeEvent(QCloseEvent *event)
{


        // 自定义对话框关闭逻辑
        // 设置返回值
        setResult(QDialog::Accepted); // 或者 QDialog::Rejected
        // 调用父类方法以确保正常关闭
        QDialog::closeEvent(event);

}

void AddFriendDialog::AddClicked(bool checked_)
{
    QString _strFriendId = ui->userIdEdit->text();
    std::string strFrinedId=_strFriendId.toStdString();
    if(strFrinedId==m_MasterUserId)
    {
        QMessageBox::warning(this,"温馨提示","您不能添加自己为好友");
        return;
    }
    MuduoClient* _pClient = MuduoClient::instance(nullptr, InetAddress());
    assert(_pClient);
    _pClient->setAddCallback(
        std::bind(&AddFriendDialog::AddCallback, this, std::placeholders::_1));
    _pClient->setErrorCallback(
        std::bind(&AddFriendDialog::ErrorCallback, this, std::placeholders::_1));

    std::string strMaster(m_MasterUserId);
    bool _bRet = _pClient->sendAddMessage(strMaster, strFrinedId);
    assert(_bRet);

    m_nMutex.lock();
    m_nAddRet = -1;
    m_bError = false;
    while (m_nAddRet == -1
        && m_bError == false)
    {
      m_nCondition.wait();
    }

    int _nAddRet = m_nAddRet;
    bool _bError = m_bError;
    m_nMutex.unlock();
    if(_bError)
    {
        ui->addRetText->setText(QString("Error"));
    }
    else
    {
        if(_nAddRet == 0)//要添加的好友不存在
        {
            ui->addRetText->setText(QString("好友不存在"));
        }
        else if(_nAddRet == 1)//添加好友成功
        {
            ui->addRetText->setText(QString("添加好友成功"));
        }
        else if(_nAddRet==2)//已经是好友，无需重复添加
        {
            ui->addRetText->setText(QString("已经是好友，无需重复添加"));
        }
    }
}

void AddFriendDialog::ExitClicked(bool checked_)
{
    QDialog::accept();
}

void AddFriendDialog::AddCallback(int nType_)
{
    MutexLockGuard lock(m_nMutex);
    m_nAddRet = nType_;
    m_nCondition.notify();
}

void AddFriendDialog::ErrorCallback(const TcpConnectionPtr&)
{
    MutexLockGuard lock(m_nMutex);
    m_bError = true;
    m_nCondition.notify();
}

AddFriendDialog::~AddFriendDialog()
{
    delete ui;
}
