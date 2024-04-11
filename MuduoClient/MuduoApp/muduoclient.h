

#ifndef MUDUO_APP_MUDUOCLIENT_H
#define MUDUO_APP_MUDUOCLIENT_H
#include "Global/header.h"
#include "codec.h"
#include "../Ui/userinfo.h"

const std::string Image_File_Path="../ChatFile/";
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


void LogOutput(const char* msg, int len);

class MuduoClient
{
public:
    // for client to server direction
    enum MessageType
    {
        REGISTER = 0,
        LOGIN = 1,
        ADD = 2,
        UPDATE_FRIEND_LIST = 3,
        OFF_LINE = 4,
        NEED_TO_PROCESS_USER_LIST = 5,
        CHAT = 6,
        GET_NEED_TO_PROCESS_MESSAGES = 7,
        UPDATE_MY_INFO=8,
        MODIFY_MY_INFO=9,
        SEND_PHOTO=10,
        GET_NEED_TO_PROCESS_PHOTO_MESSAGE=11,
    };

    // for server to client direction
    enum ResMessageType
    {
        REGISTER_RES = 0,
        LOGIN_RES = 1,
        ADD_RES = 2,
        UPDATE_FRIEND_LIST_RES = 3,
        NEED_TO_PROCESS_USER_LIST_RES = 4,
        CHAT_RES = 5,
        NEW_MESSAGE_NOTIFICATION = 6,
        GET_NEED_TO_PROCESS_MESSAGES_RES = 7,
        UPDATE_MY_INFO_RES=8,
        MODIFY_MY_INFO_RES=9,
        SEND_PHOTO_RES=10,
        GET_NEED_TO_PROCESS_PHOTO_MESSAGE_RES=11,
    };



public:
    // For Error
    void setErrorCallback(std::function<void(const TcpConnectionPtr&)> nFun_)
    {
        if(m_nConnection)
        {
            m_nConnection.get()->setErrCallback(nFun_);
        }
    }

private:
    std::function<void()> m_nErrorCallback;

public:
    // For Register
    bool sendRegisterMessage(const std::string& strUserId_, const std::string& strPassword_);
    void setRegisterCallback(std::function<void(int nType_)> nFun_)
    {
        m_nRegisterCallback = nFun_;
    }

private:
    bool PackRegisterMessage(const std::string& strUserId_, const std::string& strPassword_, Buffer& nBuf_);

private:
    std::function<void(int nType_)> m_nRegisterCallback;


public:
    // For get-need-to-process-messages
    bool sendGetNeedToProcessMessagesMessage(const QString& strSourceUserId_, const QString& strDestUserId_);
    void setGetNeedToProcessMessagesCallback(std::function<void(const NDataStruct::DynArray<Message>&)> nFun_)
    {
        m_nGetNeedToProcessMessagesCallback = nFun_;
    }

private:
    bool PackGetNeedToProcessMessagesMessage(const QString& strUserId_, const QString& strPassword_, Buffer& nBuf_);

private:
    std::function<void(const NDataStruct::DynArray<Message>&)> m_nGetNeedToProcessMessagesCallback;





public:
    // For Chat
    bool sendChatMessage(
        const std::string& nByteSender_,
        const std::string& nByteReceiver,
        const std::string& strChatText_);
    void setChatCallback(std::function<void(int nType_)> nFun_)
    {
        m_nChatCallback = nFun_;
    }

private:
    bool PackChatMessage(
        const std::string& nByteSender_,
        const std::string& nByteReceiver,
        const std::string& strChatText_,
        Buffer& nBuf_);

private:
    std::function<void(int nType_)> m_nChatCallback;
public:
    //for get need to process photo message
    bool sendGetNeedToProcessPhotoMessageMessage(const QString& strSourceUserId_, const QString& strDestUserId_ );
    void setGetNeedToProcessPhotoMessagesCallback(std::function<void (const NDataStruct::DynArray<PhotoMessage>&)> nFun_ )
    {
        m_nGetNeedToProcessPhotoMessagesCallback=nFun_;
    }

private:
    bool PackGetNeedToProcessPhotoMessageMessage(const QString& strUserId_, const QString& strPassword_, Buffer& nBuf_);

private:
     std::function<void(const NDataStruct::DynArray<PhotoMessage>&)> m_nGetNeedToProcessPhotoMessagesCallback;

public:
    // for update friend list
    bool sendUpdateFriendListMessage(const std::string& strUserId_);
    void setUpdateFriendListCallback(std::function<void(const NDataStruct::DynArray<std::string>&)> nFun_)
    {
        m_nUpdateFriendListCallback = nFun_;
    }

private:
    bool PackUpdateFriendListMessage(const std::string& strUserId_, Buffer& nBuf_);

private:
    std::function<void(const NDataStruct::DynArray<std::string>&)> m_nUpdateFriendListCallback;


public:
    // for need-to-process-user list
    bool sendNeedToProcessUserListMessage(const QString& strUserId_);
    void setNeedToProcessUserListCallback(std::function<void(const NDataStruct::DynArray<QByteArray>&)> nFun_)
    {
        m_nNeedToProcessUserListCallback = nFun_;
    }

private:
    bool PackNeedToProcessUserListMessage(const QString& strUserId_, Buffer& nBuf_);

private:
    std::function<void(const NDataStruct::DynArray<QByteArray>&)> m_nNeedToProcessUserListCallback;


public:
    void setNewMessageCallback(std::function<void(const QByteArray&)> nFun_)
    {
        m_nNewMessageCallback = nFun_;
    }

private:
    std::function<void(const QByteArray&)> m_nNewMessageCallback;



public:
    // for off-line notification
    bool sendOffLineMessage(const QString& strUserId_, const TimeStamp& nTimeStamp_);

private:
    bool PackOffLineMessage(const QString& strUserId_, const TimeStamp& nTimeStamp_, Buffer& nBuf_);


public:
    // For Login
    bool sendLoginMessage(const QString& strUserId_, const QString& strPassword_);
    void setLoginCallback(std::function<void(int nType_, TimeStamp nTimeStamp_)> nFun_)
    {
        m_nLoginCallback = nFun_;
    }

private:
    bool PackLoginMessage(const QString& strUserId_, const QString& strPassword_, Buffer& nBuf_);

private:
    std::function<void(int nType_, TimeStamp nTimeStamp_)> m_nLoginCallback;

public:
    // For Add
    bool sendAddMessage(const std::string& strMasterUserId_, const std::string& strUserId_);
    void setAddCallback(std::function<void(int nType_)> nFun_)
    {
        m_nAddCallback = nFun_;
    }

private:
    bool PackAddMessage(const std::string& strMasterUserId_, const std::string& strUserId_, Buffer& nBuf_);

private:
    std::function<void(int nType_)> m_nAddCallback;

public:
    //for update my personal information by student id;
    bool sendUpdateMyInfoMessage(std::string studentId);

    void setUpdateMyInfoCallback(std::function<void (UserInfo latestInfo)> cb)
    {
        m_UpdateMyInfoCallback=cb;
    }
private:
    bool PackUpdateMyInfoMessage(std::string studentId,Buffer& n_buf);
private:
   std::function<void (UserInfo latestInfo)> m_UpdateMyInfoCallback; 


public:
   bool sendModifyMyInfoMessage(UserInfo newInfo);
   void setModifyMyInfoCallback(std::function<void (int ret)> cb)
   {
       m_ModifyMyInfoCallbak=cb;
   }

private:
       bool PackSendModifyMyInfoMessage(UserInfo newInfo,Buffer& n_buf);
private:
   std::function<void (int ret)>  m_ModifyMyInfoCallbak;

public:
   bool sendPhotoMessage(
           const std::string& nByteSender_,
           const std::string& nByteReceiver,
           const std::string& strImgName,
           const std::string& PixPhoto_);

   void setSendPhotoMessageCallback(std::function<void (int ret)> cb)
   {
       m_PhotoMessageCallback=cb;
   }

private:
   bool packPhotoMessage (const std::string& nByteSender_,
                         const std::string& nByteReceiver,
                         const std::string& strImgName,
                          const std::string strPhoto_,
                          Buffer& buf
                         );
private:
   std::function <void (int ret)> m_PhotoMessageCallback;


public:
    static MuduoClient* instance(EventLoop* loop, const InetAddress& serverAddr);
    void connect();
    void disconnect();
    //void write(const StringPiece& message);

private:
    void onConnection(const TcpConnectionPtr& conn);
    void onStringMessage(
          const TcpConnectionPtr&,
          const string& message,
          TimeStamp);

    void onMessage(
          const TcpConnectionPtr& conn,
          Buffer* buf,
          TimeStamp receiveTime);
    void onStringMessage(
        const TcpConnectionPtr&,
        char* strMessage_,
        int nLength_,
        TimeStamp);
private:
    MuduoClient(EventLoop* loop, const InetAddress& serverAddr);

private:
    static MuduoClient* __instance;

private:
    TcpClient m_nClient;
    //LengthHeaderCodec m_nCodec;
    MutexLock m_nMutex;
    TcpConnectionPtr m_nConnection;
};

#endif // MUDUOCLIENT_H
