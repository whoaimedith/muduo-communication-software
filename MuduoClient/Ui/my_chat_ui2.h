#ifndef MY_CHAT_UI2_H
#define MY_CHAT_UI2_H
#include "header.h"
#include <QMainWindow>
#include <QListView>
#include <QStandardItem>
#include <QListWidget>
#include "userinformation.h"
#include "addfrienddialog.h"
#include "customitemwidget.h"
#include "peerchat.h"
#include <QFileDialog>

namespace Ui {
class My_Chat_UI2;
}

class My_Chat_UI2 : public QMainWindow
{
    Q_OBJECT

private:
    explicit My_Chat_UI2(QWidget *parent = nullptr);
     My_Chat_UI2(My_Chat_UI2& m )=delete;
public:
    static My_Chat_UI2* instance_MainChatUI();
     std::string  get_UserId(){return m_UserId;}
     TimeStamp get_TimeStamp() {return m_nLoginTime;};
  void    set_UserId(std::string userId){m_UserId=userId;}
  void    set_TimeStamp(TimeStamp loginTime){m_nLoginTime=loginTime;}



    void Initialize();
    virtual void timerEvent(QTimerEvent *event);
    bool addMessage(QListWidget* listWidget,QString msg,QPixmap img, bool headId);
    void UpdateNeedToProcessUserShow(const NDataStruct::DynArray<QByteArray>& arrUsers_);


    void NewMessageCallback(const QByteArray& nByteSender_);
    bool     GetMyInformation();

    void OffLine();//离线
    virtual void closeEvent(QCloseEvent *event);
    ~My_Chat_UI2();
signals:
    void InitMyInfo();
private slots:
    void onInitMyInfo();
public :
    void UpdateFriendList();
    void UpdateFriendCallback(const NDataStruct::DynArray<std::string>& arrUserIds_ );

private:

    void GetNeedToProcessMessagesCallback(const NDataStruct::DynArray<Message>& arrMessages_);

    void GetNeedToProcessPhotoMessagesCallback(const NDataStruct::DynArray<PhotoMessage>& arrPhotoMessages_);

private slots:
    void on_toolButton_3_clicked();

    void on_toolButton_2_clicked();

    void on_toolButton_4_clicked();


    void on_toolButton_5_clicked();



    void on_toolButton_clicked();

    void on_toolButton_8_clicked();
private slots:

    void onItemClicked(QListWidgetItem *item);
    void onFriendItemDoubleClicked(QListWidgetItem *item);


private:
    static My_Chat_UI2*  self;

    Ui::My_Chat_UI2 *ui;
    UserInformation *userinfo;

    std::map<std::string ,PeerChat* > m_chatWidget_Map;//映射学号到聊天窗口；



    mutable MutexLock m_nMutexLogin;
    std::string m_UserId;//我的学号
    TimeStamp m_nLoginTime;

    std::string m_chattingFriendId;

    mutable MutexLock m_nMutex;
    Condition m_nCondition;
    bool m_bError;
    bool m_bFriendValid;
    NDataStruct::DynArray<std::string> m_arrFriends;

    mutable MutexLock m_nMutexForGetNeedToProcessMessage;
    Condition m_nConditionForGetNeedToProcessMessage;
    bool m_bGetNeedToProcessMessageError;
    int m_nGetNeedToProcessMessagesRet;

    mutable MutexLock m_nMutexForGetNeedToProcessPhotoMessage;
    Condition m_nConditionForGetNeedToProcessPhotoMessage;
    bool m_bGetNeedToProcessPhotoMessageError;
    int m_nGetNeedToProcessPhotoMessageRet;

    mutable MutexLock m_nMutexForNeedToProcessUsers;
    Condition m_nConditionForNeedToProcessUsers;
    bool m_bNeedToProcessUserValid;
    NDataStruct::DynArray<QByteArray> m_arrNeedToProcessUsers;

    int m_nRefreshTimerId;
};

#endif // MY_CHAT_UI2_H
