

#ifndef MUDUO_APP_MUDUOSERVER_H
#define MUDUO_APP_MUDUOSERVER_H

#include "header.h"
#include "codec.h"
#include "MySqlAgent/ConnectionPool.h"
#include <QDataStream>
#include <QPixmap>
#include <QBuffer>
#include <QTemporaryFile>
#include <iostream>
#include <fstream>
// TcpServer的用户通过TcpServer提供的接口来使用TcpServer的功能
// 内部的高效运作，正确性管理属于网络库的责任
//
// 提供的接口包含：
// １．开启服务器的监听
// ２．设置收到已经连接套接字信息时的回调
// ３．设置服务器的属性
// ４．关闭服务器
// 与客户端一样
// 因为一般TCP的
// 消息接收需要解码再交给应用
// 消息发送需要编码再交给TcpConnection
// 所以可TcpClient一样，TcpServer没有提供给TcpConnection发消息接口
// 应用层需要自己实现
// 原始消息－－编码－－发给TcpConnection

class Message
{
public:
    Message()
    {
        m_bDirection = true;
    }

    ~Message()
    {

    }

public:
    std::string m_nContent;
    bool m_bDirection;
    TimeStamp m_nTimeStamp;
};

class PhotoMessage
{
public:
    PhotoMessage()
    {
        m_bDirection=true;
    }

    ~PhotoMessage()
    {

    }
public:
    std::string m_photoName;
    std::string m_photoMessage;
    bool m_bDirection;
    TimeStamp m_nTimeStamp;


};
class MuduoServer
{
public:
    // for client to server direction
    enum MessageType
    {
        REGISTER = 0,                           //注册
        LOGIN = 1,                              //登录
        ADD = 2,                                //添加好友
        UPDATE_FRIEND_LIST = 3,                 //更新好友列表
        OFF_LINE = 4,                           //离线
        NEED_TO_PROCESS_USER_LIST = 5,          //需要处理用户列表
        CHAT = 6,                               //聊天
        GET_NEED_TO_PROCESS_MESSAGES = 7,       //获取需要处理的消息
        UPDATE_MY_INFO=8,                          //更新个人信息
        MODIFY_MY_INFO=9,                       //修改个人信息
        SEND_PHOTO=10,
        GET_NEED_TO_PROCESS_PHOTO_MESSAGE=11,
    };

    // for server to client direction
    enum ResMessageType
    {
        REGISTER_RES = 0,                           //注册应答
        LOGIN_RES = 1,                              //登录应答
        ADD_RES = 2,                                //添加好友应答
        UPDATE_FRIEND_LIST_RES = 3,                 //更新好友列表应答
        NEED_TO_PROCESS_USER_LIST_RES = 4,          //需要处理用户列表应答
        CHAT_RES = 5,                               //聊天应答
        NEW_MESSAGE_NOTIFICATION = 6,               //新消息通知
        GET_NEED_TO_PROCESS_MESSAGES_RES = 7,       //获取需要处理的消息应答
        UPDATE_MY_INFO_RES=8,                       //更新个人信息应答
        MODIFY_MY_INFO_RES=9,                       //修改个人信息应答
        SEND_PHOTO_RES=10,
        GET_NEED_TO_PROCESS_PHOTO_MESSAGE_RES=11,
    };

public:
    MuduoServer(
        EventLoop* loop,
        const InetAddress& listenAddr,
        char* pStrHost_,
        int nHostLen_,
        char* pStrUser_,
        int nUserLen_,
        char* pStrPassword_,
        int nPasswordLen_,
        int max_sql_connections
        );
    // tcp server with multiple threads consisting of thread pool
    void setThreadNum(int numThreads);
    // create socket and start listen, at the same starting watching the event of readable
    void start();
private:
    void threadInit(EventLoop* loop);
    // 不论连接回调由服务器端那个线程触发
    // 执行的回调函数都是这个
    // 且由于成员函数性质，所有线程执行回调时，
    // 均可获得一个多个线程共享的回调语境－－－一个共享的MuduoServer对象
    // the time of one client connection build and destroy
    void onConnection(const TcpConnectionPtr& conn);
    void onMessage(
        const TcpConnectionPtr& conn,
        Buffer* buf,
        TimeStamp receiveTime);

    // a place to decode the client's message
    // and dispatch the decoded message to process it
    void onStringMessage(
        const TcpConnectionPtr&,
        void* strMessage_,
        int nLength_,
        TimeStamp);

    bool ResForRegister(
        const TcpConnectionPtr& pTcpConnection_,
        int nRetCode_);
    bool PackRegisterResMessage(
        int nRetCode_,
        Buffer& nBuf_);


    bool ResForLogin(
        const TcpConnectionPtr& pTcpConnection_,
        int nRetCode_,
        TimeStamp nTimeStamp_ = TimeStamp());
    bool PackLoginResMessage(
        int nRetCode_,
        TimeStamp nTimeStamp_,
        Buffer& nBuf_);

    bool ResForAdd(
        const TcpConnectionPtr& pTcpConnection_,
        int nRetCode_);
    bool PackAddResMessage(
        int nRetCode_,
        Buffer& nBuf_);


    bool ResForUpdateFriendList(
        const TcpConnectionPtr& pTcpConnection_,
        const NDataStruct::DynArray<std::string>& arrUserIds_);
    bool PackUpdateFriendListResMessage(
        const NDataStruct::DynArray<std::string>& arrUserIds_,
        Buffer& nBuf_);


    bool ResForNeedToProcessUserList(
        const TcpConnectionPtr& pTcpConnection_,
        const NDataStruct::DynArray<QByteArray>& arrUserIds_);
    bool PackNeedToProcessUserListResMessage(
        const NDataStruct::DynArray<QByteArray>& arrUserIds_,
        Buffer& nBuf_);


    bool ResForGetNeedToProcessMessagesMessage(
        const TcpConnectionPtr& pTcpConnection_,
        const NDataStruct::DynArray<Message>& arrMessages_);
    bool PackGetNeedToProcessMessagesMessage(
        const NDataStruct::DynArray<Message>& arrMessages_,
        Buffer& nBuf_);

    bool ResForGetNeedToProcessPhotoMessagesMessage(
        const TcpConnectionPtr& pTcpConnection_,
        const NDataStruct::DynArray<PhotoMessage>& arrMessages_);
    bool PackGetNeedToProcessPhotoMessagesMessage(
        const NDataStruct::DynArray<PhotoMessage>& arrMessages_,
        Buffer& nBuf_);


    bool ResForChatMessage(
        const TcpConnectionPtr& pTcpConnection_);
    bool PackChatResMessage(
        Buffer& nBuf_);

   bool ResForPhotoMessage(
            const TcpConnectionPtr& pTcpConnection_
           );

   bool PackPhotoResMessage(
       Buffer& nBuf_);
    bool ResForNewMessageNotification(
        const TcpConnectionPtr& pTcpConnection_,
        const QByteArray& nByteSender_);
    bool PackNewResMessageNotification(
        const QByteArray& nByteSender_,
        Buffer& nBuf_);
    
    bool ResForUpdateMyInfo(const TcpConnectionPtr& pTcpConnection_,const QByteArray& nByteSenderInfo);
    bool PackUpdateMyInfo(const QByteArray& nByteSenderInfo,Buffer& nBuf_);

    bool ResForModifyMyInfo(const TcpConnectionPtr& pTcpConnection_,int ret);
    bool PackUpdateMyInfo(int ret,Buffer& nBuf_);


    // a place to pack message and sending to client
    // void PackAndSend(
    //      int nType_,
    //      );
    //void
 
 // typedef std::shared_ptr<TcpConnection> TcpConnectionPtr;
    typedef std::set<TcpConnectionPtr> ConnectionList;//TCP连接对象的智能指针列表
    
    // typedef std::weak_ptr<TcpConnection> TcpConnectionWeakPtr;
    typedef std::map<QByteArray, std::set<TcpConnectionWeakPtr>> ConnectionMapping;

    typedef NDataStruct::DynArray<std::pair<TimeStamp, TcpConnectionWeakPtr>> ComplexConnections;
    typedef std::shared_ptr<ComplexConnections> ComplexConnectionsPtr;

    typedef std::map<QByteArray, ComplexConnectionsPtr> ComplexConnectionsMapping;

    void distributeMessage(
          const string& message);

private:
    bool ProcessRegister(
        const TcpConnectionPtr& pTcpConnection_,
        char* pUserId_,
        int nUserIdLen_,
        char* pPassword_,
        int nPasswordLen_);

    bool ProcessLogin(
        const TcpConnectionPtr& pTcpConnection_,
        char* pUserId_,
        int nUserIdLen_,
        char* pPassword_,
        int nPasswordLen_);

    bool ProcessAdd(
        const TcpConnectionPtr& pTcpConnection_,
        char* pMasterUserId_,
        int nMasterUserIdLen_,
        char* pUserId_,
        int nUserIdLen_);

    bool ProcessUpdateFriendList(
        const TcpConnectionPtr& pTcpConnection_,
        char* pUserId_,
        int nUserIdLen_);

    bool ProcessNeedToProcessUserList(
        const TcpConnectionPtr& pTcpConnection_,
        char* pUserId_,
        int nUserIdLen_);

    bool ProcessChatMessage(
        const TcpConnectionPtr& pTcpConnection_,
        char* pSenderUserId_,
        int nSenderUserIdLen_,
        char* pReceiverUserId_,
        int nReceiverUserIdLen_,
        char* pChatContent_,
        int nChatContentLen_);

    // Process Photo-Message
    bool ProcessPhotoMessage(
            const TcpConnectionPtr& pTcpConnection_,
            char* pSenderUserId_,
            int nSenderUserIdLen_,
            char* pReceiverUserId_,
            int nReceiverUserIdLen_,
       char* _strImageName,
        int _nImageNameLen,
        char*  _strPhoto,
       int  _nStrPhotoLen   );

    bool ProcessGetNeedToProcessMessagesMessage(
        const TcpConnectionPtr& pTcpConnection_,
        char* pSenderUserId_,
        int nSenderUserIdLen_,
        char* pReceiverUserId_,
        int nReceiverUserIdLen_);
    bool ProcessGetNeedToProcessPhotoMessagesMessage(
        const TcpConnectionPtr& pTcpConnection_,
        char* pSenderUserId_,
        int nSenderUserIdLen_,
        char* pReceiverUserId_,
        int nReceiverUserIdLen_);
    
    bool ProcessUpdateMyInfo(const TcpConnectionPtr& pTcpConnection_,char* pStudentId,int nStudentIdLen);
bool    ProcessModifyMyInfo(
      const TcpConnectionPtr& pTcpConnection_,
      char* _StudentId,
      int _nStudentIdLen,
    char* _Name,
                int _nNameLen,
                char* _Gender,
               int _nGenderLen,
                char* _School,
               int  _nSchoolLen
       );
    bool ProcessOffLine(
        const TcpConnectionPtr& pTcpConnection_,
        char* pUserId_,
        int nUserIdLen_,
        const TimeStamp& nTimeStamp_);

private:
    TcpServer m_nServer;
    MySqlAgent m_nSqlAgent;
    ConnectionPool m_sqlpool;
    //LengthHeaderCodec m_nCodec;

    // every thread in the thread pool has a ConnectionList object
    typedef ThreadLocalSingleton<ConnectionList> LocalConnections;

    MutexLock m_nMutex;
    // every thread in the thread pool has a EventLoop object
    std::set<EventLoop*> m_nLoops;


    MutexLock m_nMutexForMapping;
    ComplexConnectionsMapping m_mapComplexConnectionsMapping;
};

#endif // MUDUOSERVER_H
