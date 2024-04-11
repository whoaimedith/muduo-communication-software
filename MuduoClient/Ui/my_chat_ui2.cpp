#include "my_chat_ui2.h"
#include "ui_my_chat_ui2.h"
//#include <QStandardItem>
#include <QLabel>
//#include "ChatItemDelegate.h"
#include "peerchat.h"
#include "chatitem.h"
#include "registerandloginwidget.h"


My_Chat_UI2*  My_Chat_UI2::self=nullptr;

void addChatRecords();

// 创建自定义的QWidget，只显示图像
class CustomItemWidget2 : public QWidget {
public:
    CustomItemWidget2(const QIcon &icon,QWidget *parent = nullptr) : QWidget(parent) {
        QHBoxLayout *layout = new QHBoxLayout(this);
        QLabel *iconLabel = new QLabel(this);
        iconLabel->setPixmap(icon.pixmap(50, 50)); // 设置头像图标
        // 创建一个弹簧
        QSpacerItem *leftSpacer = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);
        QSpacerItem *rightSpacer = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);
        // 在布局中添加左侧弹簧
       // layout->addItem(leftSpacer);
        // 在布局中添加label
        layout->addWidget(iconLabel, 0, Qt::AlignCenter);
        // 在布局中添加右侧弹簧
        //layout->addItem(rightSpacer);
    }
};


My_Chat_UI2::My_Chat_UI2(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::My_Chat_UI2),userinfo(new UserInformation),m_nCondition(m_nMutex),

    m_nConditionForGetNeedToProcessMessage(m_nMutexForGetNeedToProcessMessage),

    m_nConditionForGetNeedToProcessPhotoMessage(m_nMutexForGetNeedToProcessPhotoMessage),
    m_nConditionForNeedToProcessUsers(m_nMutexForNeedToProcessUsers)
{
    ui->setupUi(this);
    m_nRefreshTimerId=startTimer(1000);

    userinfo->hide();
    connect(this,&My_Chat_UI2::InitMyInfo,this,&My_Chat_UI2::onInitMyInfo);

    ui->stackedWidget->setCurrentIndex(0);
    // 设置第一项为选中状态
    ui->listWidget_4->setCurrentRow(0);
    //单选
    ui->listWidget_4->setSelectionMode(QAbstractItemView::SingleSelection);

    // 将 QListWidget 的 currentRowChanged 信号与槽函数关联
        QObject::connect(ui->listWidget_4, &QListWidget::currentRowChanged, ui->stackedWidget, &QStackedWidget::setCurrentIndex);

        connect(ui->listWidget_4, &QListWidget::itemClicked, this, &My_Chat_UI2::onItemClicked);
        connect(ui->listWidget_2,&QListWidget::itemDoubleClicked,this,&My_Chat_UI2::onFriendItemDoubleClicked);
    ui->listWidget->setStyleSheet("QListWidget { background-color: rgb(246, 245, 244); }");
    ui->listWidget_2->setStyleSheet("QListWidget { background-color: rgb(246, 245, 244); }");
    ui->listWidget_3->setStyleSheet("QListWidget { background-color: rgb(246, 245, 244); }");
   // ui->listWidget_4->setStyleSheet("QListWidget { background-color: rgb(222, 221, 218) }");
    //ui->listWidget_4->setStyleSheet("QListWidget { border: none; ");
    //ui->listWidget_4->setStyleSheet("QListWidget::item:selected { background-color: lightblue; }");






    // 创建三个QListWidgetItem，并设置每个项的图标

        QListWidgetItem *item7 = new QListWidgetItem(ui->listWidget_4);
        CustomItemWidget2 *customWidget7 = new CustomItemWidget2(QIcon(":/image/Resouces/temp_image/png-0007.png"), ui->listWidget_4);
        item7->setSizeHint(customWidget7->sizeHint());
        ui->listWidget_4->setItemWidget(item7, customWidget7);

        QListWidgetItem *item8 = new QListWidgetItem(ui->listWidget_4);
        CustomItemWidget2 *customWidget8 = new CustomItemWidget2(QIcon(":/image/Resouces/temp_image/png-0008.png"), ui->listWidget_4);
        item8->setSizeHint(customWidget8->sizeHint());
        ui->listWidget_4->setItemWidget(item8, customWidget8);

        QListWidgetItem *item9 = new QListWidgetItem(ui->listWidget_4);
        CustomItemWidget2 *customWidget9 = new CustomItemWidget2(QIcon(":/image/Resouces/temp_image/png-0009.png"), ui->listWidget_4);
        item9->setSizeHint(customWidget9->sizeHint());
        ui->listWidget_4->setItemWidget(item9, customWidget9);


    // 给第一页 的ListWidget添加自定义的item
    QListWidgetItem *item1 = new QListWidgetItem(ui->listWidget);
    CustomItemWidget *customWidget1 = new CustomItemWidget(QIcon(":/image/Resouces/temp_image/png-0001.png"), "User1", ui->listWidget);
    item1->setSizeHint(customWidget1->sizeHint()); // 设置item的大小
    ui->listWidget->setItemWidget(item1, customWidget1);


    QListWidgetItem *item2 = new QListWidgetItem(ui->listWidget);
    CustomItemWidget *customWidget2 = new CustomItemWidget(QIcon(":/image/Resouces/temp_image/png-0002.png"), "User2", ui->listWidget);
    item2->setSizeHint(customWidget2->sizeHint()); // 设置item的大小
    ui->listWidget->setItemWidget(item2, customWidget2);

    // 给第2页 的ListWidget添加自定义的item
    QListWidgetItem *item3 = new QListWidgetItem(ui->listWidget_2);
    CustomItemWidget *customWidget3 = new CustomItemWidget(QIcon(":/image/Resouces/temp_image/png-0003.png"),    "User3", ui->listWidget_2);
    item3->setSizeHint(customWidget3->sizeHint()); // 设置item的大小
    ui->listWidget_2->setItemWidget(item3, customWidget3);

    QListWidgetItem *item4 = new QListWidgetItem(ui->listWidget_2);
    CustomItemWidget *customWidget4 = new CustomItemWidget(QIcon(":/image/Resouces/temp_image/png-0004.png"),    "User4", ui->listWidget_2);
    item4->setSizeHint(customWidget4->sizeHint()); // 设置item的大小
    ui->listWidget_2->setItemWidget(item4, customWidget4);


    // 给第3页 的ListWidget添加自定义的item
    QListWidgetItem *item5 = new QListWidgetItem(ui->listWidget_3);
    CustomItemWidget *customWidget5 = new CustomItemWidget(QIcon(":/image/Resouces/temp_image/png-0005.png"),    "User5", ui->listWidget_3);
    item5->setSizeHint(customWidget5->sizeHint()); // 设置item的大小
    ui->listWidget_3->setItemWidget(item5, customWidget5);

    QListWidgetItem *item6 = new QListWidgetItem(ui->listWidget_3);
    CustomItemWidget *customWidget6 = new CustomItemWidget(QIcon(":/image/Resouces/temp_image/png-0006.png"),    "User6", ui->listWidget_3);
    item6->setSizeHint(customWidget6->sizeHint()); // 设置item的大小
    ui->listWidget_3->setItemWidget(item6, customWidget6);

    MuduoClient* _pClient = MuduoClient::instance(nullptr, InetAddress());
    assert(_pClient);
    _pClient->setGetNeedToProcessMessagesCallback(
        std::bind(&My_Chat_UI2::GetNeedToProcessMessagesCallback, this, std::placeholders::_1));


    //聊天框设计
   /* // 创建模型和视图
       QStandardItemModel model;
       QListView *my_chat_rect=ui->listView;
       my_chat_rect->setModel(&model);
       //my_chat_rect->setItemDelegate(new ChatItemDelegate());

       // 添加聊天记录
       QList<QStandardItem *> items;
       for (int i = 0; i < 10; ++i)
       {
           QStandardItem *item = new QStandardItem("Hello, this is a message from friend " + QString::number(i));
           item->setData(false, Qt::UserRole); // 标记为好友的消息

           items.append(item);
       }
       model.appendColumn(items);

       for (int i = 0; i < 10; ++i)
       {
           QStandardItem *item = new QStandardItem("Hello, this is my message " + QString::number(i));
           item->setData(true, Qt::UserRole); // 标记为我的消息
           items.append(item);
       }
       model.appendColumn(items);

       my_chat_rect->show();
*/

/* PeerChat *pChat1=new PeerChat(this);
    pChat1->setGeometry(370,0,630,500);
    pChat1->setMaximumSize(630,500);
    pChat1->setMaximumSize(630,500);
    ChatModel::Message newMessage;
    newMessage.text="hahahahahah";
    newMessage.fromUser1=true;
    pChat1->getModel()->messages.append(newMessage);
    ChatModel::Message newMessage2;
    newMessage2.text="我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥";
    newMessage2.fromUser1=false;
    pChat1->getModel()->messages.append(newMessage2);
    pChat1-> getModel()->layoutChanged(); // 通知视图更新显示

*/

}

My_Chat_UI2 *My_Chat_UI2::instance_MainChatUI()
{
    {if (self==nullptr)
        {
            self=new My_Chat_UI2();
            return self;
        }
     else
        {
            return self;
        }



    }

}

void My_Chat_UI2::Initialize()
{
    //RegisterAndLoginWidget* _pWidget = new RegisterAndLoginWidget(this);

}

void My_Chat_UI2::timerEvent(QTimerEvent *event)
{
    if(event->timerId() == m_nRefreshTimerId)
    {
        m_nMutexForNeedToProcessUsers.lock();
        NDataStruct::DynArray<QByteArray> _arrUsers = m_arrNeedToProcessUsers;
        m_nMutexForNeedToProcessUsers.unlock();
        UpdateNeedToProcessUserShow(_arrUsers);
    }
}

bool My_Chat_UI2::addMessage(QListWidget *listWidget, QString msg, QPixmap img, bool headId)
{


                ChatItem* item = new ChatItem(msg, img,headId, this);

                QListWidgetItem* listItem = new QListWidgetItem(listWidget);
                listItem->setSizeHint(item->size());
                listWidget->setItemWidget(listItem, item);
                listWidget->update();


}

void My_Chat_UI2::UpdateNeedToProcessUserShow(const NDataStruct::DynArray<QByteArray> &arrUsers_)
{
    ui->listWidget->clear();
    for(int i=0;i<arrUsers_.GetSize();++i)
    {
    // 给第一页 的ListWidget添加自定义的item
    QListWidgetItem *item1 = new QListWidgetItem(ui->listWidget);
    CustomItemWidget *customWidget1 = new CustomItemWidget(QIcon(":/image/Resouces/temp_image/png-0001.png"), arrUsers_[i], ui->listWidget);
    item1->setSizeHint(customWidget1->sizeHint()); // 设置item的大小
    ui->listWidget->setItemWidget(item1, customWidget1);
    }

    QByteArray _nUserId = QString::fromStdString(m_chattingFriendId).toUtf8();
    int _nIndex = arrUsers_.Find([_nUserId](const QByteArray& nByte_)->bool{
        return (_nUserId == nByte_);
    });
    if(_nIndex == -1)
    {
        return;
    }

    // a send b a message
    // in database b's needtoprocessfriends table insert a

    // master is b
    QString _strSourceUserId = QString::fromStdString(m_UserId).toUtf8();
    // dest is a
    QString _strDestUserId = _nUserId;

    MuduoClient* _pClient = MuduoClient::instance(nullptr, InetAddress());
    bool _bRet = _pClient->sendGetNeedToProcessMessagesMessage(_strSourceUserId, _strDestUserId);
    assert(_bRet);


    m_nMutexForGetNeedToProcessMessage.lock();
    m_nGetNeedToProcessMessagesRet = -1;
    m_bGetNeedToProcessMessageError = false;
    std::cout<<__FILE__<<"  :" <<__LINE__<<std::endl<<std::flush;
    while (m_nGetNeedToProcessMessagesRet == -1

        && m_bGetNeedToProcessMessageError == false)
    {
        std::cout<<__FILE__<<"  :" <<__LINE__<<std::endl<<std::flush;
      m_nConditionForGetNeedToProcessMessage.wait();
    }
std::cout<<__FILE__<<"  :" <<__LINE__<<std::endl<<std::flush;
    m_chatWidget_Map[m_chattingFriendId]->UpdateShow();
    std::cout<<__FILE__<<"  :" <<__LINE__<<std::endl<<std::flush;
    m_nMutexForGetNeedToProcessMessage.unlock();


std::cout<<__FILE__<<"  :" <<__LINE__<<std::endl<<std::flush;
    _pClient->setGetNeedToProcessPhotoMessagesCallback(std::bind(&My_Chat_UI2::GetNeedToProcessPhotoMessagesCallback,this,std::placeholders::_1));
        _bRet=    _pClient->sendGetNeedToProcessPhotoMessageMessage(_strSourceUserId,_strDestUserId);
std::cout<<__FILE__<<"  :" <<__LINE__<<std::endl<<std::flush;
    assert(_bRet);
    m_nMutexForGetNeedToProcessPhotoMessage.lock();
    std::cout<<__FILE__<<"  :" <<__LINE__<<std::endl<<std::flush;
    m_nGetNeedToProcessPhotoMessageRet = -1;
    m_bGetNeedToProcessPhotoMessageError = false;
    while (m_nGetNeedToProcessPhotoMessageRet == -1
        && m_bGetNeedToProcessPhotoMessageError == false)
    {
        std::cout<<__FILE__<<"  :" <<__LINE__<<std::endl<<std::flush;
      m_nConditionForGetNeedToProcessPhotoMessage.wait();
      std::cout<<__FILE__<<"  :" <<__LINE__<<std::endl<<std::flush;
    }
    std::cout<<__FILE__<<"  :" <<__LINE__<<std::endl<<std::flush;
    m_chatWidget_Map[m_chattingFriendId]->UpdatePhotoShow();
    m_nMutexForGetNeedToProcessPhotoMessage.unlock();
std::cout<<__FILE__<<"  :" <<__LINE__<<std::endl<<std::flush;

    m_nMutexForNeedToProcessUsers.lock();
    std::cout<<__FILE__<<"  :" <<__LINE__<<std::endl<<std::flush;
    m_arrNeedToProcessUsers.DeleteByValue(QString::fromStdString(m_chattingFriendId).toUtf8());
    std::cout<<__FILE__<<"  :" <<__LINE__<<std::endl<<std::flush;
    m_nMutexForNeedToProcessUsers.unlock();

}

void My_Chat_UI2::NewMessageCallback(const QByteArray &nByteSender_)
{
    std::cout<<__LINE__<<"进入NewMessageCallbak()"<<std::endl<<std::flush;
    m_nMutexForNeedToProcessUsers.lock();
    int _nIndex = m_arrNeedToProcessUsers.Find([nByteSender_](const QByteArray& nByteUser_)->bool {
        return (nByteUser_ == nByteSender_);
    });
    if(_nIndex == -1)
    {
        m_arrNeedToProcessUsers.Add(nByteSender_);
    }

    m_nMutexForNeedToProcessUsers.unlock();
    std::cout<<__LINE__<<"结束NewMessageCallbak()"<<std::endl<<std::flush;
    std::cout<<__LINE__<<"m_arrNeedToProcessUsers里的内容为："<<std::endl<<std::flush;
    for(int i=0;i<m_arrNeedToProcessUsers.GetSize();++i)
    {
        std::cout<<QString(m_arrNeedToProcessUsers[i]).toStdString()<<std::endl<<std::flush;
    }
}

bool My_Chat_UI2::GetMyInformation()
{

}

void My_Chat_UI2::OffLine()
{
    MuduoClient* _pClient = MuduoClient::instance(nullptr, InetAddress());
    assert(_pClient);
    bool _bRet = _pClient->sendOffLineMessage(QString::fromStdString(m_UserId), m_nLoginTime);
    assert(_bRet);
    // just send the notification, not need to wait for res.
}

void My_Chat_UI2::closeEvent(QCloseEvent *event)
{
    OffLine();
}


My_Chat_UI2::~My_Chat_UI2()
{
    delete ui;
    delete userinfo;
}

void My_Chat_UI2::onInitMyInfo()
{
    userinfo->setUserID(get_UserId());
    std::cout<<"即将执行更新个人信息"<<std::endl<<std::flush;
    userinfo->UpdateMyInfo();
}

void My_Chat_UI2::UpdateFriendList()
{
    MuduoClient* _pClient = MuduoClient::instance(nullptr, InetAddress());
    assert(_pClient);
    _pClient->setUpdateFriendListCallback(
        std::bind(&My_Chat_UI2::UpdateFriendCallback, this, std::placeholders::_1));
    //_pClient->setErrorCallback(
    //    std::bind(&MainChatWidget::ErrorCallback, this, std::placeholders::_1));
    bool _bRet = _pClient->sendUpdateFriendListMessage(m_UserId);
    assert(_bRet);

    m_nMutex.lock();
    m_arrFriends.DeleteAll();
    m_bFriendValid = false;
    m_bError = false;
    while (m_bFriendValid == false
        && m_bError == false)
    {
      m_nCondition.wait();
    }

    NDataStruct::DynArray<std::string> _arrFriends = m_arrFriends;
    bool _bError = m_bError;
    m_nMutex.unlock();
    if(_bError)
    {
        assert(false);
    }
    else
    {//显示所有好友

        std::cout<<"查询到的好友信息为："<<std::endl<<std::flush;
        for(int i = 0; i < _arrFriends.GetSize(); i++)
        {
            std::cout<<_arrFriends[i]<<",  "<<std::flush;
        }
        std::cout<<std::endl<<std::flush;
        ui->listWidget_2->clear();
        for(int i=0;i<_arrFriends.GetSize();++i)
        {
        QListWidgetItem *item4 = new QListWidgetItem(ui->listWidget_2);
        CustomItemWidget *customWidget4 = new CustomItemWidget(QIcon(":/image/Resouces/temp_image/png-0004.png"),    _arrFriends[i].c_str(), ui->listWidget_2);
        item4->setSizeHint(customWidget4->sizeHint()); // 设置item的大小
        ui->listWidget_2->setItemWidget(item4, customWidget4);
        }
    }
}

void My_Chat_UI2::UpdateFriendCallback(const NDataStruct::DynArray<std::string>& arrUserIds_)
{
    MutexLockGuard lock(m_nMutex);
    m_arrFriends = arrUserIds_;
    m_bFriendValid = true;
    m_nCondition.notify();
}

void My_Chat_UI2::GetNeedToProcessMessagesCallback(const NDataStruct::DynArray<Message> &arrMessages_)
{
    MutexLockGuard lock(m_nMutexForGetNeedToProcessMessage);
    if(!m_chattingFriendId.empty())
    {
        std::queue<Message> queueMessage;
        for(int i=0;i<arrMessages_.GetSize();++i)
        {
            queueMessage.push(arrMessages_[i]);
        }
        m_chatWidget_Map[m_chattingFriendId]->GetNeedToProcessMessagesCallback(queueMessage);
    }

    m_nGetNeedToProcessMessagesRet = 0;
    m_bGetNeedToProcessMessageError=false;
    m_nConditionForGetNeedToProcessMessage.notify();
}

void My_Chat_UI2::GetNeedToProcessPhotoMessagesCallback(const NDataStruct::DynArray<PhotoMessage> &arrPhotoMessages_)
{
    MutexLockGuard lock(m_nMutexForGetNeedToProcessPhotoMessage);
    if(!m_chattingFriendId.empty())
    {
        std::queue<PhotoMessage> queuePhotoMessage;
        for(int i=0;i<arrPhotoMessages_.GetSize();++i)
        {
            queuePhotoMessage.push(arrPhotoMessages_[i]);
        }
        m_chatWidget_Map[m_chattingFriendId]->GetNeedToProcessPhotoMessageCallback(queuePhotoMessage);
    }

    m_nGetNeedToProcessPhotoMessageRet = 0;
    m_bGetNeedToProcessPhotoMessageError=false;
    m_nConditionForGetNeedToProcessPhotoMessage.notify();

}
void My_Chat_UI2::on_toolButton_2_clicked()
{



}


void My_Chat_UI2::on_toolButton_3_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}



void My_Chat_UI2::on_toolButton_4_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}


void My_Chat_UI2::on_toolButton_5_clicked()
{
   /* PeerChat* m_chat =new PeerChat(this);
    m_chat->setGeometry(370,0,630,500);
    m_chat->setStyleSheet("QWidget{background-color:lightblue");
    m_chat->show();

    QString str("sjfklsjdfkljsaklf;js");
    QPixmap img(":/image/Resouces/CustomerService.png");
    addMessage(m_chat->getListWidget(),str,img,0);
    QString str2("我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥我是帅哥");
    addMessage(m_chat->getListWidget(),str2,img,1);
    addMessage(m_chat->getListWidget(),str2,img,0);
    addMessage(m_chat->getListWidget(),str2,img,1);
    addMessage(m_chat->getListWidget(),str,img,0);
    addMessage(m_chat->getListWidget(),str,img,0);
    addMessage(m_chat->getListWidget(),str,img,0);
    addMessage(m_chat->getListWidget(),str,img,0);

    addMessage(m_chat->getListWidget(),str,img,0);
    addMessage(m_chat->getListWidget(),str,img,0);
    addMessage(m_chat->getListWidget(),str,img,0);
    addMessage(m_chat->getListWidget(),str,img,0);

*/
}


void My_Chat_UI2::on_toolButton_clicked()
{
    userinfo->setUserID(get_UserId());
    std::cout<<"即将执行更新个人信息"<<std::endl<<std::flush;
    userinfo->UpdateMyInfo();
    std::cout<<"即将执行展示个人信息界面"<<std::endl<<std::flush;
    userinfo->show();

}


void My_Chat_UI2::on_toolButton_8_clicked()
{
    AddFriendDialog *_pAddFriendDialog = new AddFriendDialog(this);
    _pAddFriendDialog->SetMasterUserId(m_UserId);
    int _nRet = _pAddFriendDialog->exec();
    if(_nRet == QDialog::Accepted)
    {
        UpdateFriendList();
    }

    delete _pAddFriendDialog;
}

void My_Chat_UI2::onItemClicked(QListWidgetItem *item)
{
    int index = ui->listWidget_4->row(item); // 获取点击项的索引
       if (index == 1) { // 如果点击的是第二项，即好友列表
           UpdateFriendList();
       }

}

void My_Chat_UI2::onFriendItemDoubleClicked(QListWidgetItem *item)
{
      CustomItemWidget *customWidget = qobject_cast<CustomItemWidget*>(ui->listWidget_2->itemWidget(item));
      std::string friendId_name=customWidget->getNickName();//学号加姓名构成的字符串，学号是前8位
      std::string friendId(friendId_name.data(),8);
      std::string friendName(friendId_name.begin()+8,friendId_name.end());
      std::cout<<"点击的好友的学号为："<<friendId<<std::endl<<std::flush;
       std::cout<<"点击的好友的姓名为："<<friendName<<std::endl<<std::flush;
       std::map<std::string ,PeerChat*>::iterator it;
       it=m_chatWidget_Map.find(friendId);
       std::cout<<__LINE__<<"userinfo->getMyId(): "<<userinfo->getMyId()<<std::endl;
       if(it==m_chatWidget_Map.end())//没有找到聊天窗口
        {



           PeerChat* newChatWidget=new PeerChat(userinfo->getMyId(),userinfo->getMyName(),friendId,friendName,this);
           m_chatWidget_Map[friendId]=newChatWidget;



       }

           for(it=m_chatWidget_Map.begin();it!=m_chatWidget_Map.end();++it)
           {
               if(it->first!=friendId)
               {
                    it->second->hide();

               }
               else
               {
                    m_chattingFriendId=it->first;
                   it->second->show();
               }
           }

       }









