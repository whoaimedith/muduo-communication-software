
#ifndef APP_UI_ADDFRIENDDIALOG_H
#define APP_UI_ADDFRIENDDIALOG_H

#include "header.h"
#include <QMessageBox>

namespace Ui {
class AddFriendDialog;
}

class AddFriendDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddFriendDialog(QWidget *parent = nullptr);
    ~AddFriendDialog();

    void Initialize();
    void SetMasterUserId(const std::string& MasterUserId_){m_MasterUserId = MasterUserId_;}

//private:
//    void UpdateFriendCallback(NDataStruct::DynArray<QByteArray> arrUserIds_);

    void closeEvent(QCloseEvent *event) ;
private slots:
    void AddClicked(bool checked_);
    void ExitClicked(bool checked_);

private:
    void AddCallback(int nType_);
    void ErrorCallback(const TcpConnectionPtr&);

private:
    mutable MutexLock m_nMutex;
    Condition m_nCondition;
    int m_nAddRet;
    bool m_bError;

    std::string m_MasterUserId;
private:
    Ui::AddFriendDialog *ui;
};

#endif // ADDFRIENDDIALOG_H
