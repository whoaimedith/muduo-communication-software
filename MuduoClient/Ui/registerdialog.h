#ifndef REGISTERDIALOG_H
#define REGISTERDIALOG_H
#include "header.h"
#include <QDialog>

namespace Ui {
class RegisterDialog;
}

class RegisterDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RegisterDialog(QWidget *parent = nullptr);
    bool isStringAllDigits(const std::string& str) ;
    ~RegisterDialog();

private slots:
    void on_RegisterButton_clicked();
private:
    void RegisterCallback(int nType_);
    void ErrorCallback(const TcpConnectionPtr&);

private:
    Ui::RegisterDialog *ui;
    mutable MutexLock m_nMutex;
    Condition m_nCondition;
    int m_nRegisterRet;
    bool m_bError;
};

#endif // REGISTERDIALOG_H
