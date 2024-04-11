#ifndef PEERCHAT_H
#define PEERCHAT_H

#include <QWidget>
#include <QStandardItem>
#include <QListWidget>
#include "SmoothScrollListWidget.h"
#include "../MyMuduo/Lib/lib.h"
#include "../MuduoApp/muduoclient.h"
#include <QFileDialog>
#include <queue>
#include "pictureitem.h"
#include <QMessageBox>
#include <QBuffer>

namespace Ui {
class PeerChat;
}

class PeerChat : public QWidget
{
    Q_OBJECT

public:
    explicit PeerChat(std::string selfId,std::string selfName,std::string friendId,std::string friendName,QWidget *parent = nullptr);

    QListWidget* getListWidget()const{return p_ListWidget;}
    void addMessage(QString msg, QPixmap img, bool headId);
    void addImage( QPixmap picImg, QPixmap headImg, bool headId);
    void UpdateShow();
    void UpdatePhotoShow();

    void FormatShowText(std::queue<Message> arrHistoryMessage);
     void FormatShowPhotoMessage(std::queue<PhotoMessage> arrHistoryPhotoMessage);

     void GetNeedToProcessMessagesCallback(const std::queue<Message>& arrMessages_);
     void GetNeedToProcessPhotoMessageCallback(const std::queue<PhotoMessage>& arrPhotoMessages_);

     void SendPhotoMessageCallback(int ret);

    ~PeerChat();
    void ErrorCallback(const TcpConnectionPtr&);
    void PhotoErrorCallback(const TcpConnectionPtr &);
private:
     void ChatCallback(int nType_);
private slots:
    void showList()
    {


    }
    void on_toolButton_5_clicked();

    void on_toolButton_2_clicked();

private:
    Ui::PeerChat *ui;
    SmoothScrollListWidget* p_ListWidget;
    std::string m_selfId;
    std::string m_selfName;
    std::string m_friendId;
    std::string m_friendName;

    mutable MutexLock m_nMutex;
    Condition m_nCondition;
    int m_nChatRet;
    bool m_bError;

    mutable MutexLock m_nPhotoMutex;
    Condition m_nPhotoCondition;
    int m_nPhotoRet;
    bool m_bPhotoError;

    mutable MutexLock m_nMutexChatMessage;
    std::queue<Message>  m_arrChatMessage;

    mutable MutexLock m_nMutexPhotoMessage;
    std::queue<PhotoMessage> m_arrPhotoMessage;

};

#endif // PEERCHAT_H
