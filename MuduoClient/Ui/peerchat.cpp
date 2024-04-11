#include "peerchat.h"
#include "ui_peerchat.h"
#include <QListWidget>
//#include "ChatItemDelegate.h"
//#include "chat_model.h"
#include "chatitem.h"
//PeerChat::PeerChat(QWidget *parent) :
 //   QWidget(parent),
 //   ui(new Ui::PeerChat)
//{
   //ui->setupUi(this);
   //this->setGeometry(370,0,630,750);
   //p_ListWidget=new SmoothScrollListWidget(this);


   //p_ListWidget->setGeometry(0,20,630,480);
   //p_ListWidget->setMinimumSize(630,480);
   //p_ListWidget->setMaximumSize(630,480);
   //p_ListWidget->setStyleSheet("QListWidget{background-color:lightblue;}");


    //p_ListView->setItemDelegate(new ChatItemDelegate());
    //QStandardItemModel *m_model=new QStandardItemModel(this);
    //QStandardItem *item=new QStandardItem();
    //item->setData("hahahahaha",Qt::DisplayRole);
    //item->setData(false,Qt::UserRole);
    //m_model->appendRow(item);
    //p_ListView->setModel(m_model);
    //model_ = new ChatModel();
    //p_ListView->setModel(model_);
    //p_ListView->setItemDelegate(new ChatItemDelegate());

//}


PeerChat::PeerChat(std::string selfId,std::string selfName,std::string friendId, std::string friendName, QWidget *parent)
                :
                QWidget(parent),
                ui(new Ui::PeerChat),
                m_selfId(selfId),
                m_selfName(selfName),
                m_friendId(friendId),
                m_friendName(friendName),
                m_nCondition(m_nMutex),
                m_nPhotoCondition(m_nPhotoMutex)
{
    ui->setupUi(this);
    this->setGeometry(370,0,630,750);
    p_ListWidget=new SmoothScrollListWidget(this);
    QLabel * friendLabel=new QLabel(this);
    friendLabel->setText(QString::fromStdString(m_friendId+m_friendName));
    friendLabel->setGeometry(0,0,630,20);
    friendLabel->setAlignment(Qt::AlignHCenter);



    p_ListWidget->setGeometry(0,20,630,480);
    p_ListWidget->setMinimumSize(630,480);
    p_ListWidget->setMaximumSize(630,480);
    p_ListWidget->setStyleSheet("QListWidget{background-color:lightblue;}");

}

void PeerChat::UpdateShow()
{
    ui->textEdit->clear();//TODO::check delete?
    m_nMutexChatMessage.lock();
    std::queue<Message> _arrChatMessage = m_arrChatMessage;
    std::queue<Message>::size_type queue_size=m_arrChatMessage.size();
    for(std::queue<Message>::size_type  i=0;i<queue_size;++i)
    {
        m_arrChatMessage.pop();
    }
    m_nMutexChatMessage.unlock();
    FormatShowText(_arrChatMessage);

}
void PeerChat::UpdatePhotoShow()
{
    std::cout<<__FILE__<<"  :" <<__LINE__<<std::endl<<std::flush;

    std::cout<<__FILE__<<"  :" <<__LINE__<<"current thread: " <<tid()<< "isLockedByThisThread:"<<m_nMutexPhotoMessage.isLockedByThisThread()<<  std::endl<<std::flush;

    m_nMutexPhotoMessage.lock();
    std::cout<<__FILE__<<"  :" <<__LINE__<<"current thread: " <<tid()<< "isLockedByThisThread:"<<m_nMutexPhotoMessage.isLockedByThisThread()<<  std::endl<<std::flush;
    std::cout<<__FILE__<<"  :" <<__LINE__<<std::endl<<std::flush;
    std::queue<PhotoMessage> _arrPhotoMessage =m_arrPhotoMessage;
    std::queue<PhotoMessage>::size_type queue_size=m_arrPhotoMessage.size();
    for(std::queue<PhotoMessage>::size_type i=0;i<queue_size;++i)
    {
        m_arrPhotoMessage.pop();
    }
    m_nMutexPhotoMessage.unlock();
    std::cout<<__FILE__<<"  :" <<__LINE__<<std::endl<<std::flush;
    FormatShowPhotoMessage(_arrPhotoMessage);
}

void PeerChat::FormatShowText(std::queue<Message> arrHistoryMessage)
{
   // p_ListWidget->clear();
    std::queue<Message>::size_type queue_size=arrHistoryMessage.size();
    for(std::queue<Message>::size_type i=0;i<queue_size;++i)
    {
        Message it=arrHistoryMessage.front();
        arrHistoryMessage.pop();

        if(it.m_bDirection)//1代表我的的消息，靠右显示
        {
             addMessage(QString::fromStdString(it.m_nContent),QPixmap(":/image/Resouces/CustomerService.png"),it.m_bDirection);

        }
        else
            {
            addMessage(QString::fromStdString(it.m_nContent),QPixmap(":/image/Resouces/Customer Copy.png"),it.m_bDirection);

            }

    }
}

void PeerChat::FormatShowPhotoMessage(std::queue<PhotoMessage> arrHistoryPhotoMessage)
{
    std::queue<Message>::size_type queue_size=arrHistoryPhotoMessage.size();
    for(std::queue<Message>::size_type i=0;i<queue_size;++i)
    {
        PhotoMessage it=arrHistoryPhotoMessage.front();
        std::string imgName=it.m_photoName;
        std::string imageData=it.m_photoMessage;
        // 将std::string转换为QByteArray
           QByteArray byteArray(imageData.data(), static_cast<int>(imageData.size()));

           // 将QByteArray转换为QImage
           QImage image;
           image.loadFromData(byteArray);

           // 将QImage转换为QPixmap
           QPixmap pixmap = QPixmap::fromImage(image);

           // 检查转换是否成功
           if (pixmap.isNull()) {
               qDebug() << "Failed to convert string to QPixmap";
           } else {
               qDebug() << "Successfully converted string to QPixmap";
           }
        arrHistoryPhotoMessage.pop();
std::cout<<__FILE__<<"  :" <<__LINE__<<std::endl<<std::flush;
        if(it.m_bDirection)//1代表我的的消息，靠右显示
        {
            std::cout<<__FILE__<<"  :" <<__LINE__<<std::endl<<std::flush;

std::cout<<__FILE__<<"  :" <<__LINE__<<std::endl<<std::flush;
             addImage(pixmap,QPixmap(":/image/Resouces/CustomerService.png"),it.m_bDirection);

        }
        else
            {
            std::cout<<__FILE__<<"  :" <<__LINE__<<std::endl<<std::flush;
            addImage(pixmap,QPixmap(":/image/Resouces/Customer Copy.png"),it.m_bDirection);

            }

    }
}


void PeerChat::GetNeedToProcessMessagesCallback(const std::queue<Message> &arrMessages_)
{
    m_nMutexChatMessage.lock();
    m_arrChatMessage=arrMessages_;
    m_nMutexChatMessage.unlock();

}

void PeerChat::GetNeedToProcessPhotoMessageCallback(const std::queue<PhotoMessage> &arrPhotoMessages_)
{
    MutexLockGuard lock(m_nMutexPhotoMessage);
    m_arrPhotoMessage=arrPhotoMessages_;
     std::cout<<__FILE__<<"  :" <<__LINE__<<"  " <<arrPhotoMessages_.size()<<std::endl<<std::flush;

}

void PeerChat::SendPhotoMessageCallback(int ret)
{
    MutexLockGuard lock(m_nPhotoMutex);
    std::cout<<__FILE__<<"  :" <<__LINE__<<std::endl<<std::flush;
    m_nPhotoRet=ret;
    std::cout<<__FILE__<<"  :" <<__LINE__<<" ret= "<<ret<<std::endl<<std::flush;
    m_nPhotoCondition.notify();



}

PeerChat::~PeerChat()
{
    delete ui;
}

void PeerChat::ErrorCallback(const TcpConnectionPtr &)
{
    MutexLockGuard lock(m_nMutex);
    m_bError = true;
    m_nCondition.notify();
}

void PeerChat::PhotoErrorCallback(const TcpConnectionPtr &)
{
    MutexLockGuard lock(m_nPhotoMutex);
    m_bPhotoError =true;
    m_nPhotoCondition.notify();
}

void PeerChat::ChatCallback(int nType_)
{
    MutexLockGuard lock(m_nMutex);
    m_nChatRet = nType_;
    m_nCondition.notify();
}

void PeerChat::on_toolButton_5_clicked()//消息发送按钮点击；
{
    QString _strText = ui->textEdit->toPlainText();
    std::string strText=_strText.toStdString();

    MuduoClient* _pClient = MuduoClient::instance(nullptr, InetAddress());
    assert(_pClient);

    _pClient->setChatCallback(
        std::bind(&PeerChat::ChatCallback, this, std::placeholders::_1));
    _pClient->setErrorCallback(
        std::bind(&PeerChat::ErrorCallback, this, std::placeholders::_1));

    bool _bRet = _pClient->sendChatMessage(m_selfId, m_friendId, strText);
    std::cout<<__LINE__<<"m_selfId: "<<m_selfId<<std::endl<<std::flush;
    assert(_bRet);

    m_nMutex.lock();
    m_nChatRet = -1;
    m_bError = false;
    while (m_nChatRet == -1
        && m_bError == false)
    {
      m_nCondition.wait();
    }

    int _nChatRet = m_nChatRet;
    bool _bError = m_bError;
    m_nMutex.unlock();
    if(_bError)
    {
        assert(false);
    }
    else
    {
        if(_nChatRet == 0)
        {
            Message _nMessage;
            _nMessage.m_nContent =strText;
            _nMessage.m_bDirection = true;
            _nMessage.m_nTimeStamp = TimeStamp::now();
            m_nMutexChatMessage.lock();
            m_arrChatMessage.push(_nMessage);
            m_nMutexChatMessage.unlock();
            UpdateShow();
        }
    }
}

void PeerChat::addMessage( QString msg, QPixmap img, bool headId)
{


                ChatItem* item = new ChatItem(msg, img,headId, this);

                QListWidgetItem* listItem = new QListWidgetItem(p_ListWidget);
                listItem->setSizeHint(item->size());
                p_ListWidget->setItemWidget(listItem, item);
                p_ListWidget->update();

}

void PeerChat::on_toolButton_2_clicked() //发送图片按钮点击事件
{
    MuduoClient* _pClient = MuduoClient::instance(nullptr, InetAddress());
    assert(_pClient);

    _pClient->setSendPhotoMessageCallback(
        std::bind(&PeerChat::SendPhotoMessageCallback, this, std::placeholders::_1));


    QString imagePath = QFileDialog::getOpenFileName(this, tr("选择图片"), "", tr("图片文件 (*.png *.jpg *.bmp)"));
    if (!imagePath.isEmpty()) {
        qDebug() << "已选择图片：" << imagePath;
        // 在这里可以使用所选图片的路径执行你的操作
        std::string imgPath=imagePath.toStdString();
       std::string   imgName = imagePath.section("/", -1).toStdString(); // 提取文件名
   // if(std::string(imgPath.end()-4,imgPath.end())==".jpg")
    //{
        std::cout<< "图片"<<imgName<<"is sending..."<<std::endl<<std::flush;
        QPixmap chooseImg(QString::fromStdString(imgPath));
        QPixmap headImg(":/image/Resouces/CustomerService.png");

        if(imgName.size()>255)
        {
             QMessageBox::critical(nullptr, "Error", "the length of image Name can not bigger than 255 characters,please adjust its size. ");
             return;
        }
        std::cout<<__FILE__<<"  :" <<__LINE__<<std::endl<<std::flush;
        QString suffix = QFileInfo(imagePath).suffix().toLower();

            // 将QPixmap转换为字节数组
            QByteArray byteArray;
            QBuffer buffer(&byteArray);
            buffer.open(QIODevice::WriteOnly);
            chooseImg.save(&buffer, suffix.toStdString().c_str()); // 根据后缀名保存图片
            std::string photoMessage=byteArray.toStdString();
std::cout<<__FILE__<<"  :" <<__LINE__<<std::endl<<std::flush;
        bool _bRet = _pClient->sendPhotoMessage(m_selfId, m_friendId, imgName,photoMessage);

        assert(_bRet);
std::cout<<__FILE__<<"  :" <<__LINE__<<std::endl<<std::flush;

        m_nPhotoMutex.lock();
        std::cout<<__FILE__<<"  :" <<__LINE__<<std::endl<<std::flush;
        m_nPhotoRet = -1;
        m_bPhotoError = false;
        while (m_nPhotoRet == -1
            && m_bPhotoError == false)
        {
            std::cout<<__FILE__<<"  :" <<__LINE__<<std::endl<<std::flush;
          m_nPhotoCondition.wait();
          std::cout<<__FILE__<<"  :" <<__LINE__<<std::endl<<std::flush;
        }
std::cout<<__FILE__<<"  :" <<__LINE__<<std::endl<<std::flush;
        int _nPhotoRet = m_nPhotoRet;
        bool _bPhotoError = m_bPhotoError;
        std::cout<<__FILE__<<"  :" <<__LINE__<<"_nPhotoRet = "<<_nPhotoRet<<std::endl<<std::flush;
        m_nPhotoMutex.unlock();
        if(_bPhotoError)
        {
            assert(false);
        }
        else
        {
            if(_nPhotoRet == 0)
            {
                std::cout<<__FILE__<<"  :" <<__LINE__<<"_nPhotoRet= " <<_nPhotoRet<<std::endl<<std::flush;
                PhotoMessage _nMessage;
                _nMessage.m_photoName=imgName;
                _nMessage.m_photoMessage=photoMessage;
                _nMessage.m_nTimeStamp = TimeStamp::now();



                m_nMutexPhotoMessage.lock();
                std::cout<<__FILE__<<"  :" <<__LINE__<<std::endl<<std::flush;
                m_arrPhotoMessage.push(_nMessage);
                m_nMutexPhotoMessage.unlock();

                std::cout<<__FILE__<<"  :" <<__LINE__<<std::endl<<std::flush;
                UpdatePhotoShow();
                std::cout<<__FILE__<<"  :" <<__LINE__<<std::endl<<std::flush;

            }

    }
    //} else {
     //   qDebug() << "取消选择图片";
   // }
    }
}

void PeerChat::addImage( QPixmap picImg, QPixmap headImg, bool headId)
{


                PictureItem* item = new PictureItem(picImg, headImg,headId, this);

                QListWidgetItem* listItem = new QListWidgetItem(p_ListWidget);
                listItem->setSizeHint(item->size());
                p_ListWidget->setItemWidget(listItem, item);
               // (static_cast<QListWidget*>(p_ListWidget))->resizeRowsToContents();
                p_ListWidget->update();

}
