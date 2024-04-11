#include "registerwidget.h"
#include "ui_registerwidget.h"
#include <QMessageBox>
registerwidget::registerwidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::registerwidget)
{
    ui->setupUi(this);
}

registerwidget::~registerwidget()
{
    delete ui;
}

void registerwidget::on_RegisterButton_clicked()
{

   /* MuduoClient* _pClient = MuduoClient::instance(nullptr, InetAddress());
    assert(_pClient);
    // set Register callback to process the register result
    // set Error callback to process error
    // not concern the timeout callback
    _pClient->setRegisterCallback(
        std::bind(&RegisterAndLoginWidget::RegisterCallback, this, std::placeholders::_1));
    _pClient->setErrorCallback(
        std::bind(&RegisterAndLoginWidget::ErrorCallback, this, std::placeholders::_1));

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
        ui->infoLabel->setText(QString("Error"));
    }
    else
    {
        if(_nRegisterRet == 0)
        {
            ui->infoLabel->setText(QString("Success"));
        }
        else if(_nRegisterRet == 1)
        {
            ui->infoLabel->setText(QString("Fail"));
        }
        else
        {
            ui->infoLabel->setText(QString("Exception"));
        }
    } */
}

