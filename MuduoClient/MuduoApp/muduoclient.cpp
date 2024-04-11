
#include "muduoclient.h"

void LogOutput(const char* msg, int len)
{
  static int fd = -1;
  if(fd == -1)
  {
      fd = open("log.txt", O_RDWR | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR);
  }

  write(fd, msg, len);
  fsync(fd);
}

MuduoClient* MuduoClient::__instance = nullptr;

MuduoClient* MuduoClient::instance(EventLoop* loop, const InetAddress& serverAddr)
{
    if(__instance == nullptr)
    {
        __instance = new MuduoClient(loop, serverAddr);
    }

    return __instance;
}

// for login
bool MuduoClient::sendLoginMessage(const QString& strUserId_, const QString& strPassword_)
{
    Buffer _nBuf;
    bool _bRet = PackLoginMessage(strUserId_, strPassword_, _nBuf);
    if(!_bRet)
    {
        return _bRet;
    }

    MutexLockGuard lock(m_nMutex);
    if (m_nConnection)
    {
        m_nConnection.get()->send(&_nBuf);
    }
    else
    {
        return false;
    }

    return true;
}

bool MuduoClient::PackLoginMessage(const QString& strUserId_, const QString& strPassword_, Buffer& nBuf_)
{
    // pack and send to server
    // information format
    // 4 byte for length, thus we can send a message at most 2^32-1 byte
    // 2 byte for type, thus we can process 2^16-1 different types.
    // for login type
    // the content format
    // 1 byte for user-id length, thus we can process a user-id at most 2^8-1 byte
    // 1 byte for password length, thus we can process a password at most 2^8-1 byte
    QByteArray _nUserId = strUserId_.toUtf8();
    QByteArray _nPassword = strPassword_.toUtf8();
    int _nLen1 = _nUserId.length();
    if(_nLen1 >= 256)
    {
        return false;
    }

    int _nLen2 = _nPassword.length();
    if(_nLen2 >= 256)
    {
        return false;
    }

    int _nLength = 4 + 2 + 1 + _nLen1 + 1 + _nLen2;
    char _strMessage[_nLength - 4];
    memset(_strMessage, 0, sizeof(_strMessage));
    // for type
    int16_t _nType = LOGIN;
    int16_t _nNetType = hostToNetwork16(_nType);
    char* _pPos = _strMessage;
    memcpy(_pPos, &_nNetType, 2);

    _pPos += 2;
    // for user-id length
    int8_t _nUserIdLen = _nLen1;
    memcpy(_pPos, &_nUserIdLen, 1);

    // for user-id
    _pPos += 1;
    memcpy(_pPos, _nUserId.data(), _nUserIdLen);

    _pPos += _nUserIdLen;
    // for password length
    int8_t _nPasswordLen = _nLen2;
    memcpy(_pPos, &_nPasswordLen, 1);

    _pPos += 1;
    // for password
    memcpy(_pPos, _nPassword.data(), _nPasswordLen);

    nBuf_.append(
        _strMessage,
        _nLength-4);

    int32_t len = static_cast<int32_t>(_nLength);
    int32_t be32 = hostToNetwork32(len);
    nBuf_.prepend(
        &be32,
        sizeof be32);
    return true;
}

// for add
bool MuduoClient::sendAddMessage(const std::string& strMasterUserId_, const std::string& strUserId_)
{
    Buffer _nBuf;
    bool _bRet = PackAddMessage(strMasterUserId_, strUserId_, _nBuf);
    if(!_bRet)
    {
        return _bRet;
    }

    MutexLockGuard lock(m_nMutex);
    if (m_nConnection)
    {
        m_nConnection.get()->send(&_nBuf);
    }
    else
    {
        return false;
    }

    return true;
}

bool MuduoClient::PackAddMessage(const std::string& strMasterUserId_, const std::string& strUserId_, Buffer& nBuf_)
{
    // pack and send to server
    // information format
    // 4 byte for length, thus we can send a message at most 2^32-1 byte
    // 2 byte for type, thus we can process 2^16-1 different types.
    // for add type
    // the content format
    // 1 byte for user-id length, thus we can process a user-id at most 2^8-1 byte
    // 1 byte for user-id length, thus we can process a user-id at most 2^8-1 byte

    int _nLen0 = strMasterUserId_.length();
    if(_nLen0 >= 256)
    {
        return false;
    }


    int _nLen1 = strUserId_.length();
    if(_nLen1 >= 256)
    {
        return false;
    }

    int _nLength = 4 + 2 + 1 + _nLen0 + 1 + _nLen1;
    char _strMessage[_nLength - 4];
    memset(_strMessage, 0, sizeof(_strMessage));
    // for type
    int16_t _nType = ADD;
    int16_t _nNetType = hostToNetwork16(_nType);
    char* _pPos = _strMessage;
    memcpy(_pPos, &_nNetType, 2);

    _pPos += 2;

    // for master-user-id length
    int8_t _nMasterUserIdLen = _nLen0;
    memcpy(_pPos, &_nMasterUserIdLen, 1);

    // for master-user-id
    _pPos += 1;
    memcpy(_pPos, strMasterUserId_.data(), _nMasterUserIdLen);

    _pPos += _nMasterUserIdLen;

    // for user-id length
    int8_t _nUserIdLen = _nLen1;
    memcpy(_pPos, &_nUserIdLen, 1);

    // for user-id
    _pPos += 1;
    memcpy(_pPos, strUserId_.data(), _nUserIdLen);

    nBuf_.append(
        _strMessage,
        _nLength-4);

    int32_t len = static_cast<int32_t>(_nLength);
    int32_t be32 = hostToNetwork32(len);
    nBuf_.prepend(
        &be32,
        sizeof be32);
    return true;
}

bool MuduoClient::sendUpdateMyInfoMessage(std::string studentId)
{
    Buffer _nBuf;
    bool _bRet = PackUpdateMyInfoMessage(studentId, _nBuf);
    if(!_bRet)
    {
        return _bRet;
    }

    MutexLockGuard lock(m_nMutex);
    if (m_nConnection)
    {
        m_nConnection.get()->send(&_nBuf);
    }
    else
    {
        return false;
    }

    return true;

}

bool MuduoClient::PackUpdateMyInfoMessage(std::string studentId, Buffer &nbuf_)
{
    // pack and send to server
    // information format
    // 4 byte for length, thus we can send a message at most 2^32-1 byte
    // 2 byte for type, thus we can process 2^16-1 different types.
    // for add type
    // the content format
    // 1 byte for user-id length, thus we can process a user-id at most 2^8-1 byte
    // 1 byte for user-id length, thus we can process a user-id at most 2^8-1 byte

    int _nLen0 = studentId.length();
    if(_nLen0 >= 256)
    {
        return false;
    }

   

    int _nLength = 4 + 2 + 1 + _nLen0 ;
    char _strMessage[_nLength - 4];
    memset(_strMessage, 0, sizeof(_strMessage));
    // for type
    int16_t _nType = UPDATE_MY_INFO;
    int16_t _nNetType = hostToNetwork16(_nType);
    char* _pPos = _strMessage;
    memcpy(_pPos, &_nNetType, 2);

    _pPos += 2;


    int8_t studentIdLen = _nLen0;
    memcpy(_pPos, &studentIdLen, 1);

    // for master-user-id
    _pPos += 1;
    memcpy(_pPos, studentId.c_str(), studentIdLen);


  

    nbuf_.append(
        _strMessage,
        _nLength-4);

    int32_t len = static_cast<int32_t>(_nLength);
    int32_t be32 = hostToNetwork32(len);
    nbuf_.prepend(
        &be32,
        sizeof be32);
    return true;
}

bool MuduoClient::sendModifyMyInfoMessage(UserInfo newInfo)
{
    Buffer _nBuf;
    bool _bRet = PackSendModifyMyInfoMessage(newInfo, _nBuf);
    if(!_bRet)
    {
        return _bRet;
    }

    MutexLockGuard lock(m_nMutex);
    if (m_nConnection)
    {
        m_nConnection.get()->send(&_nBuf);
    }
    else
    {
        return false;
    }

    return true;


}

bool MuduoClient::PackSendModifyMyInfoMessage(UserInfo newInfo, Buffer &n_buf)
{
    //QByteArray modifiedInfo;
    std::cout<<"进入PackSendModifyMyInfoMessage"<<std::endl<<std::flush;
    std::string studentId=newInfo.getStudentID();
    std::cout<<"studentid: "<<studentId<<"length: "<<studentId.length()<<std::endl<<std::flush;
     int8_t studentIdLen=studentId.length();

    std::string name=newInfo.getName();
     int8_t nameLen=name.length();
    std::cout<<"name:"<<name<<"  length: "<<name.length()<<std::endl<<std::flush;

    std::string gender=newInfo.getGender();
     int8_t genderLen=gender.length();
    std::cout<<"gender: "<<gender<<"length: "<<gender.length()<<std::endl<<std::flush;

    std::string school=newInfo.getSchool();
     int8_t schoolLen=school.length();
     std::cout<<"school:"<<school <<" length"<<school.length()<<std::endl<<std::flush;
     int _nLength = 4 + 2 +  +1+studentIdLen+1+nameLen+1+genderLen+1+schoolLen ;
     std::cout<<"_nLength:"<<_nLength<<std::endl<<std::flush;


    char strMessage[_nLength-4];
    memset(strMessage,0,sizeof strMessage);
    //char _strMessage[_nLength - 4];
    memset(strMessage, 0, sizeof(strMessage));
    // for type
    int16_t _nType = MODIFY_MY_INFO;
    int16_t _nNetType = hostToNetwork16(_nType);
    char* _pPos = strMessage;
    memcpy(_pPos, &_nNetType, 2);

    _pPos += 2;

    memcpy(_pPos,&studentIdLen,1);
    _pPos+=1;
    memcpy(_pPos, studentId.c_str(),studentIdLen);
    _pPos+=studentIdLen;

    memcpy(_pPos,&nameLen,1);
    _pPos+=1;
    memcpy(_pPos,name.c_str(),nameLen);
    _pPos+=nameLen;

    memcpy(_pPos,&genderLen,1);
    _pPos+=1;
    memcpy(_pPos,gender.c_str(),genderLen);
    _pPos+=genderLen;

    memcpy(_pPos,&schoolLen,1);
    _pPos+=1;
    memcpy(_pPos,school.c_str(),schoolLen);
    _pPos+=schoolLen;
    n_buf.append(
        strMessage,
        _nLength-4);

    int32_t len = static_cast<int32_t>(_nLength);
    int32_t be32 = hostToNetwork32(len);
    n_buf.prepend(
        &be32,
        sizeof be32);
    return true;
}


bool MuduoClient::sendOffLineMessage(const QString& strUserId_, const TimeStamp& nTimeStamp_)
{
    Buffer _nBuf;
    bool _bRet = PackOffLineMessage(strUserId_, nTimeStamp_, _nBuf);
    if(!_bRet)
    {
        return _bRet;
    }

    MutexLockGuard lock(m_nMutex);
    if (m_nConnection)
    {
        m_nConnection.get()->send(&_nBuf);
    }
    else
    {
        return false;
    }

    return true;
}

bool MuduoClient::PackOffLineMessage(const QString& strUserId_, const TimeStamp& nTimeStamp_, Buffer& nBuf_)
{
    // pack and send to server
    // information format
    // 4 byte for length, thus we can send a message at most 2^32-1 byte
    // 2 byte for type, thus we can process 2^16-1 different types.
    // for off-line type
    // the content format
    // 1 byte for user-id length and the userid content, thus we can process a user-id at most 2^8-1 byte
    // 8 byte for timestamp
    QByteArray _nUserId = strUserId_.toUtf8();
    int _nLen1 = _nUserId.length();
    if(_nLen1 >= 256)
    {
        return false;
    }

    int _nLength = 4 + 2 + 1 + _nLen1 + 8;
    char _strMessage[_nLength - 4];
    memset(_strMessage, 0, sizeof(_strMessage));
    // for type
    int16_t _nType = OFF_LINE;
    int16_t _nNetType = hostToNetwork16(_nType);
    char* _pPos = _strMessage;
    memcpy(_pPos, &_nNetType, 2);

    _pPos += 2;
    // for user-id length
    int8_t _nUserIdLen = _nLen1;
    memcpy(_pPos, &_nUserIdLen, 1);

    // for user-id
    _pPos += 1;
    memcpy(_pPos, _nUserId.data(), _nUserIdLen);

    // for 8 byte timestamp
    _pPos += _nUserIdLen;
    int64_t _nTime = nTimeStamp_.microSecondsSinceEpoch();
    int64_t _nNetTime = hostToNetwork64(_nTime);
    memcpy(_pPos, &_nNetTime, 8);

    nBuf_.append(
        _strMessage,
        _nLength-4);

    int32_t len = static_cast<int32_t>(_nLength);
    int32_t be32 = hostToNetwork32(len);
    nBuf_.prepend(
        &be32,
        sizeof be32);
    return true;
}


bool MuduoClient::sendNeedToProcessUserListMessage(const QString& strUserId_)
{
    Buffer _nBuf;
    bool _bRet = PackNeedToProcessUserListMessage(strUserId_, _nBuf);
    if(!_bRet)
    {
        return _bRet;
    }

    MutexLockGuard lock(m_nMutex);
    if (m_nConnection)
    {
        m_nConnection.get()->send(&_nBuf);
    }
    else
    {
        return false;
    }

    return true;
}

bool MuduoClient::PackNeedToProcessUserListMessage(const QString& strUserId_, Buffer& nBuf_)
{
    // pack and send to server
    // information format
    // 4 byte for length, thus we can send a message at most 2^32-1 byte
    // 2 byte for type, thus we can process 2^16-1 different types.
    // for need-to-process-user-list type
    // the content format
    // 1 byte for user-id length, thus we can process a user-id at most 2^8-1 byte
    QByteArray _nUserId = strUserId_.toUtf8();
    int _nLen1 = _nUserId.length();
    if(_nLen1 >= 256)
    {
        return false;
    }

    int _nLength = 4 + 2 + 1 + _nLen1;
    char _strMessage[_nLength - 4];
    memset(_strMessage, 0, sizeof(_strMessage));
    // for type
    int16_t _nType = NEED_TO_PROCESS_USER_LIST;
    int16_t _nNetType = hostToNetwork16(_nType);
    char* _pPos = _strMessage;
    memcpy(_pPos, &_nNetType, 2);

    _pPos += 2;
    // for user-id length
    int8_t _nUserIdLen = _nLen1;
    memcpy(_pPos, &_nUserIdLen, 1);

    // for user-id
    _pPos += 1;
    memcpy(_pPos, _nUserId.data(), _nUserIdLen);

    nBuf_.append(
        _strMessage,
        _nLength-4);

    int32_t len = static_cast<int32_t>(_nLength);
    int32_t be32 = hostToNetwork32(len);
    nBuf_.prepend(
        &be32,
        sizeof be32);
    return true;
}


bool MuduoClient::sendUpdateFriendListMessage(const std::string& strUserId_)
{
    Buffer _nBuf;
    bool _bRet = PackUpdateFriendListMessage(strUserId_, _nBuf);
    if(!_bRet)
    {
        return _bRet;
    }

    MutexLockGuard lock(m_nMutex);
    if (m_nConnection)
    {
        m_nConnection.get()->send(&_nBuf);
    }
    else
    {
        return false;
    }

    return true;
}

bool MuduoClient::PackUpdateFriendListMessage(const std::string& strUserId_, Buffer& nBuf_)
{
    // pack and send to server
    // information format
    // 4 byte for length, thus we can send a message at most 2^32-1 byte
    // 2 byte for type, thus we can process 2^16-1 different types.
    // for update-friend-list type
    // the content format
    // 1 byte for user-id length, thus we can process a user-id at most 2^8-1 byte
    std::string _nUserId = strUserId_;
    int _nLen1 = _nUserId.length();
    if(_nLen1 >= 256)
    {
        return false;
    }

    int _nLength = 4 + 2 + 1 + _nLen1;
    char _strMessage[_nLength - 4];
    memset(_strMessage, 0, sizeof(_strMessage));
    // for type
    int16_t _nType = UPDATE_FRIEND_LIST;
    int16_t _nNetType = hostToNetwork16(_nType);
    char* _pPos = _strMessage;
    memcpy(_pPos, &_nNetType, 2);

    _pPos += 2;
    // for user-id length
    int8_t _nUserIdLen = _nLen1;
    memcpy(_pPos, &_nUserIdLen, 1);

    // for user-id
    _pPos += 1;
    memcpy(_pPos, _nUserId.data(), _nUserIdLen);

    nBuf_.append(
        _strMessage,
        _nLength-4);

    int32_t len = static_cast<int32_t>(_nLength);
    int32_t be32 = hostToNetwork32(len);
    nBuf_.prepend(
        &be32,
        sizeof be32);
    return true;
}

bool MuduoClient::sendChatMessage(
    const std::string& nByteSender_,
    const std::string& nByteReceiver,
    const std::string& strChatText_)
{
    Buffer _nBuf;
    bool _bRet = PackChatMessage(nByteSender_, nByteReceiver, strChatText_, _nBuf);
    if(!_bRet)
    {
        return _bRet;
    }

    MutexLockGuard lock(m_nMutex);
    if (m_nConnection)
    {
        m_nConnection.get()->send(&_nBuf);
    }
    else
    {
        return false;
    }

    return true;
}

bool MuduoClient::PackChatMessage(
    const std::string& nByteSender_,
    const std::string& nByteReceiver,
    const std::string& strChatText_,
    Buffer& nBuf_)
{
    // pack and send to server
    // information format
    // 4 byte for length, thus we can send a message at most 2^32-1 byte
    // 2 byte for type, thus we can process 2^16-1 different types.
    // for chat type
    // the content format
    // 1 byte for sender's user-id length, thus we can process a user-id at most 2^8-1 byte
    // 1 byte for receiver's user-id length, thus we can process a user-id at most 2^8-1 byte
    // 2 byte for the message content's length, thus we can process a content at most 2^16-1 byte
    //QByteArray _nSenderUserId = nByteSender_.toUtf8();
    //QByteArray _nReceiverUserId = nByteReceiver.toUtf8();
    //QByteArray _nByteContent = strChatText_.toUtf8();
    int _nLen1 = nByteSender_.length();
    if(_nLen1 >= 256)
    {
        return false;
    }

    int _nLen2 = nByteReceiver.length();
    if(_nLen2 >= 256)
    {
        return false;
    }

    int _nLen3 = strChatText_.length();
    if(_nLen3 >= std::pow(2, 15) - 1)
    {
        return false;
    }

    int _nLength = 4 + 2 + 1 + _nLen1 + 1 + _nLen2 + 2 + _nLen3;
    char _strMessage[_nLength - 4];
    memset(_strMessage, 0, sizeof(_strMessage));
    // for type
    int16_t _nType = CHAT;
    int16_t _nNetType = hostToNetwork16(_nType);
    char* _pPos = _strMessage;
    memcpy(_pPos, &_nNetType, 2);

    _pPos += 2;
    // for sender-user-id length
    int8_t _nUserIdLen = _nLen1;
    memcpy(_pPos, &_nUserIdLen, 1);

    // for sender-user-id
    _pPos += 1;
    memcpy(_pPos, nByteSender_.data(), _nUserIdLen);

    _pPos += _nUserIdLen;
    // for receiver-user-id length
    int8_t _nReceiverLen = _nLen2;
    memcpy(_pPos, &_nReceiverLen, 1);

    _pPos += 1;
    // for receiver-user-id
    memcpy(_pPos, nByteReceiver.data(), _nReceiverLen);


    _pPos += _nReceiverLen;
    // for content-length
    int16_t _nContentLen = _nLen3;
    int16_t _nNetContentLen = hostToNetwork16(_nContentLen);
    memcpy(_pPos, &_nNetContentLen, 2);
    // for content
    _pPos += 2;
    memcpy(_pPos, strChatText_.data(), _nContentLen);

    nBuf_.append(
        _strMessage,
        _nLength-4);

    int32_t len = static_cast<int32_t>(_nLength);
    int32_t be32 = hostToNetwork32(len);
    nBuf_.prepend(
        &be32,
        sizeof be32);
    return true;
}

bool MuduoClient::sendGetNeedToProcessPhotoMessageMessage(const QString &strSourceUserId_, const QString &strDestUserId_)
{
    Buffer _nBuf;
    bool _bRet = PackGetNeedToProcessPhotoMessageMessage(strSourceUserId_, strDestUserId_, _nBuf);
    if(!_bRet)
    {
        return _bRet;
    }

    MutexLockGuard lock(m_nMutex);
    if (m_nConnection)
    {
        m_nConnection.get()->send(&_nBuf);
    }
    else
    {
        return false;
    }

    return true;
}

bool MuduoClient::PackGetNeedToProcessPhotoMessageMessage(const QString &strUserId_, const QString &strPassword_, Buffer &nBuf_)
{
    QByteArray _nSourceUserId = strUserId_.toUtf8();
    QByteArray _nDestUserId = strPassword_.toUtf8();
    int _nLen1 = _nSourceUserId.length();
    if(_nLen1 >= 256)
    {
        return false;
    }

    int _nLen2 = _nDestUserId.length();
    if(_nLen2 >= 256)
    {
        return false;
    }

    int _nLength = 4 + 2 + 1 + _nLen1 + 1 + _nLen2;
    char _strMessage[_nLength - 4];
    memset(_strMessage, 0, sizeof(_strMessage));
    // for type
    int16_t _nType = GET_NEED_TO_PROCESS_PHOTO_MESSAGE;
    int16_t _nNetType = hostToNetwork16(_nType);
    char* _pPos = _strMessage;
    memcpy(_pPos, &_nNetType, 2);

    _pPos += 2;
    // for user-id length
    int8_t _nSourceUserIdLen = _nLen1;
    memcpy(_pPos, &_nSourceUserIdLen, 1);

    // for user-id
    _pPos += 1;
    memcpy(_pPos, _nSourceUserId.data(), _nSourceUserIdLen);

    _pPos += _nSourceUserIdLen;
    // for dest-user-id length
    int8_t _nDestUserIdLen = _nLen2;
    memcpy(_pPos, &_nDestUserIdLen, 1);

    _pPos += 1;
    // for user-id
    memcpy(_pPos, _nDestUserId.data(), _nDestUserIdLen);

    nBuf_.append(
        _strMessage,
        _nLength-4);

    int32_t len = static_cast<int32_t>(_nLength);
    int32_t be32 = hostToNetwork32(len);
    nBuf_.prepend(
        &be32,
        sizeof be32);
    return true;

}
//for photo transformation
bool MuduoClient::sendPhotoMessage(
        const std::string& nByteSender_,
        const std::string& nByteReceiver,
        const std::string& strImgName,
        const std::string& strPhoto_)
{
    Buffer _nBuf;
    bool _bRet = packPhotoMessage(nByteSender_, nByteReceiver, strImgName,strPhoto_, _nBuf);
    assert(_bRet);
    if(!_bRet)
    {
        return _bRet;
    }

    MutexLockGuard lock(m_nMutex);
    if (m_nConnection)
    {
        m_nConnection.get()->send(&_nBuf);
    }
    else
    {
        return false;
    }

    return true;

}

bool MuduoClient::packPhotoMessage (const std::string& nByteSender_,
                      const std::string& nByteReceiver,
                      const std::string& strImgName,
                       const std::string strPhoto_,
                       Buffer& buf
                      )
{
    int _nLen1 = nByteSender_.length();
    if(_nLen1 >= 256)
    {
        return false;
    }

    int _nLen2 = nByteReceiver.length();
    if(_nLen2 >= 256)
    {
        return false;
    }
//1 byte for image name size
    uint8_t _nLen3 = strImgName.length();
    if(_nLen3 >= std::pow(2, 8) - 1)
    {
        return false;
    }

    uint32_t _nLen4 =strPhoto_.length();
    if(_nLen4 >= std::pow(2,24)-1 )
    {
        return false;
    }

    uint32_t _nLength = 4 + 2 + 1 + _nLen1 + 1 + _nLen2 + 1 + _nLen3 +4 +_nLen4;
    char _strMessage[_nLength - 4];
    memset(_strMessage, 0, sizeof(_strMessage));
    // for type
    int16_t _nType = SEND_PHOTO;
    int16_t _nNetType = hostToNetwork16(_nType);
    char* _pPos = _strMessage;
    memcpy(_pPos, &_nNetType, 2);

    _pPos += 2;
    // for sender-user-id length
    int8_t _nUserIdLen = _nLen1;
    memcpy(_pPos, &_nUserIdLen, 1);

    // for sender-user-id
    _pPos += 1;
    memcpy(_pPos, nByteSender_.data(), _nUserIdLen);

    _pPos += _nUserIdLen;
    // for receiver-user-id length
    int8_t _nReceiverLen = _nLen2;
    memcpy(_pPos, &_nReceiverLen, 1);

    _pPos += 1;
    // for receiver-user-id
    memcpy(_pPos, nByteReceiver.data(), _nReceiverLen);


    _pPos += _nReceiverLen;
    // for content-length
    uint8_t _nImageNameLen = _nLen3;
    //uint8_t _nNetContentLen = hostToNetwork8(_nContentLen);
    memcpy(_pPos, &_nImageNameLen, 1);
    // for content
    _pPos += 1;
    memcpy(_pPos, strImgName.data(), _nImageNameLen);

    _pPos +=_nImageNameLen;

    uint32_t _nStrPhotoLen=_nLen4;
    uint32_t _nNetStrPhotoLen=hostToNetwork32(_nStrPhotoLen);
    memcpy(_pPos,&_nNetStrPhotoLen,4);
    _pPos +=4;

    memcpy(_pPos,strPhoto_.data(),_nStrPhotoLen);
    std::cout<<__FILE__<<__LINE__<<"_nStrPhotoLen= "<<_nStrPhotoLen<<" bytes. "<<std::endl<<std::flush;
    _pPos +=_nStrPhotoLen;

    buf.append(
        _strMessage,
        _nLength-4);

    uint32_t len = static_cast<uint32_t>(_nLength);
    uint32_t be32 = hostToNetwork32(len);
    buf.prepend(
        &be32,
        sizeof be32);
    return true;
}

// For get-need-to-process-messages
bool MuduoClient::sendGetNeedToProcessMessagesMessage(
    const QString& strSourceUserId_,
    const QString& strDestUserId_)
{
    Buffer _nBuf;
    bool _bRet = PackGetNeedToProcessMessagesMessage(strSourceUserId_, strDestUserId_, _nBuf);
    if(!_bRet)
    {
        return _bRet;
    }

    MutexLockGuard lock(m_nMutex);
    if (m_nConnection)
    {
        m_nConnection.get()->send(&_nBuf);
    }
    else
    {
        return false;
    }

    return true;
}

bool MuduoClient::PackGetNeedToProcessMessagesMessage(const QString& strSourceUserId_, const QString& strDestUserId_, Buffer& nBuf_)
{
    // pack and send to server
    // information format
    // 4 byte for length, thus we can send a message at most 2^32-1 byte
    // 2 byte for type, thus we can process 2^16-1 different types.
    // for get-need-to-process-messages
    // the content format
    // 1 byte for source-user-id length, thus we can process a user-id at most 2^8-1 byte
    // 1 byte for dest-user-id length, thus we can process a user-id at most 2^8-1 byte
    QByteArray _nSourceUserId = strSourceUserId_.toUtf8();
    QByteArray _nDestUserId = strDestUserId_.toUtf8();
    int _nLen1 = _nSourceUserId.length();
    if(_nLen1 >= 256)
    {
        return false;
    }

    int _nLen2 = _nDestUserId.length();
    if(_nLen2 >= 256)
    {
        return false;
    }

    int _nLength = 4 + 2 + 1 + _nLen1 + 1 + _nLen2;
    char _strMessage[_nLength - 4];
    memset(_strMessage, 0, sizeof(_strMessage));
    // for type
    int16_t _nType = GET_NEED_TO_PROCESS_MESSAGES;
    int16_t _nNetType = hostToNetwork16(_nType);
    char* _pPos = _strMessage;
    memcpy(_pPos, &_nNetType, 2);

    _pPos += 2;
    // for user-id length
    int8_t _nSourceUserIdLen = _nLen1;
    memcpy(_pPos, &_nSourceUserIdLen, 1);

    // for user-id
    _pPos += 1;
    memcpy(_pPos, _nSourceUserId.data(), _nSourceUserIdLen);

    _pPos += _nSourceUserIdLen;
    // for dest-user-id length
    int8_t _nDestUserIdLen = _nLen2;
    memcpy(_pPos, &_nDestUserIdLen, 1);

    _pPos += 1;
    // for user-id
    memcpy(_pPos, _nDestUserId.data(), _nDestUserIdLen);

    nBuf_.append(
        _strMessage,
        _nLength-4);

    int32_t len = static_cast<int32_t>(_nLength);
    int32_t be32 = hostToNetwork32(len);
    nBuf_.prepend(
        &be32,
        sizeof be32);
    return true;
}


// for register
bool MuduoClient::sendRegisterMessage(const std::string& strUserId_, const std::string& strPassword_)
{
    Buffer _nBuf;
    bool _bRet = PackRegisterMessage(strUserId_, strPassword_, _nBuf);
    if(!_bRet)
    {
        return _bRet;
    }

    MutexLockGuard lock(m_nMutex);
    if (m_nConnection)
    {
        m_nConnection.get()->send(&_nBuf);
    }
    else
    {
        return false;
    }

    return true;
}

bool MuduoClient::PackRegisterMessage(const std::string& strUserId_, const std::string& strPassword_, Buffer& nBuf_)
{
    // pack and send to server
    // information format
    // 4 byte for length, thus we can send a message at most 2^32-1 byte
    // 2 byte for type, thus we can process 2^16-1 different types.
    // for register type
    // the content format
    // 1 byte for user-id length, thus we can process a user-id at most 2^8-1 byte
    // 1 byte for password length, thus we can process a password at most 2^8-1 byte
    std::string _nUserId = strUserId_;
    std::string _nPassword = strPassword_;
    int _nLen1 = _nUserId.length();
    if(_nLen1 >= 256)
    {
        return false;
    }

    int _nLen2 = _nPassword.length();
    if(_nLen2 >= 256)
    {
        return false;
    }

    int _nLength = 4 + 2 + 1 + _nLen1 + 1 + _nLen2;
    char _strMessage[_nLength - 4];
    memset(_strMessage, 0, sizeof(_strMessage));
    // for type
    int16_t _nType = REGISTER;
    int16_t _nNetType = hostToNetwork16(_nType);
    char* _pPos = _strMessage;
    memcpy(_pPos, &_nNetType, 2);

    _pPos += 2;
    // for user-id length
    int8_t _nUserIdLen = _nLen1;
    memcpy(_pPos, &_nUserIdLen, 1);

    // for user-id
    _pPos += 1;
    memcpy(_pPos, _nUserId.data(), _nUserIdLen);

    _pPos += _nUserIdLen;
    // for password length
    int8_t _nPasswordLen = _nLen2;
    memcpy(_pPos, &_nPasswordLen, 1);

    _pPos += 1;
    // for password
    memcpy(_pPos, _nPassword.data(), _nPasswordLen);

    nBuf_.append(
        _strMessage,
        _nLength-4);

    int32_t len = static_cast<int32_t>(_nLength);
    int32_t be32 = hostToNetwork32(len);
    nBuf_.prepend(
        &be32,
        sizeof be32);
    return true;
}

MuduoClient::MuduoClient(EventLoop* loop, const InetAddress& serverAddr)
    : m_nClient(loop, serverAddr, "ChatClient")
{
    // set client connection's building and destroying callback
    m_nClient.setConnectionCallback(
        std::bind(
            &MuduoClient::onConnection,
            this,
            _1));

    // set client connection's receiving orginal message callback
    m_nClient.setMessageCallback(
        std::bind(
            &MuduoClient::onMessage,
            this,
            _1,
            _2,
            _3));
    // set tcp client's property to enable retry
    m_nClient.enableRetry();
}

void MuduoClient::onMessage(
      const TcpConnectionPtr& conn,
      Buffer* buf,
      TimeStamp receiveTime)
{
    size_t _nFixLength = sizeof(int32_t);
    while (buf->readableBytes() >= _nFixLength)
    {
        const void* data = buf->peek();
        uint32_t be32 = *static_cast<const uint32_t*>(data);
        const uint32_t len = networkToHost32(be32);
        if (len > 4294967295
            || len < 0)
        {
            std::cout<<__FILE__<<__LINE__<<std::endl<<std::flush;
            LOG_ERROR
                << "Invalid length "
                << len;
            conn->shutdown();
            break;
        }
        else if (buf->readableBytes() >= (size_t)len)
        {
            std::cout<<__FILE__<<__LINE__<<std::endl<<std::flush;
            buf->retrieve(_nFixLength);

            char _strMessage[len-_nFixLength];
            memcpy(_strMessage, buf->peek(), len - _nFixLength);
            // 这里应用层将协议打包和解码抽离为一个独立的类
            // 用将解码好的消息执行应用层回调
            onStringMessage(
                conn,
                _strMessage,
                len-_nFixLength,
                receiveTime);
            buf->retrieve(len - _nFixLength);
        }
        else
        {
            break;
        }
    }
}

void MuduoClient::onStringMessage(
    const TcpConnectionPtr&,
    char* pStrMessage_,
    int nLength_,
    TimeStamp)
{
    Q_UNUSED(nLength_);
    // information format
    // 2 byte for type, thus we can process 2^16-1 different types.
    // for register type
    // the content format
    // 1 byte for user-id length, thus we can process a user-id at most 2^8-1 byte
    // 1 byte for password length, thus we can process a password at most 2^8-1 byte
    const void* data = pStrMessage_;
    int16_t be16 = *static_cast<const int16_t*>(data);
    const int16_t _nType = networkToHost16(be16);
    if(_nType == REGISTER_RES)
    {
        char* _pPos = (char*)data;
        _pPos += 2;
        // for register-res type
        int8_t _nRegisterResType = *((int8_t*)_pPos);
        m_nRegisterCallback(_nRegisterResType);
    }
    else if(_nType == LOGIN_RES)
    {
        char* _pPos = (char*)data;
        _pPos += 2;
        // for login-res type
        int8_t _nLoginResType = *((int8_t*)_pPos);

        _pPos += 1;
        // for time-stamp
        int64_t _nNetTime = *((int64_t*)_pPos);
        const int64_t _nTime = networkToHost64(_nNetTime);
        TimeStamp _nTimeStamp(_nTime);
        m_nLoginCallback(_nLoginResType, _nTimeStamp);
    }
    else if(_nType == ADD_RES)
    {
        char* _pPos = (char*)data;
        _pPos += 2;
        // for add-res type
        int8_t _nAddResType = *((int8_t*)_pPos);
        m_nAddCallback(_nAddResType);
    }
    else if(_nType == UPDATE_FRIEND_LIST_RES)
    {
        char* _pPos = (char*)data;
        _pPos += 2;
        // for friend number
        int8_t _nFriendNum = *((int8_t*)_pPos);
        _pPos += 1;
        NDataStruct::DynArray<std::string> _arrFriends;
        for(int i = 0; i < _nFriendNum; i++)
        {
            // for user-id length
            int8_t _nUserIdLength = *((int8_t*)_pPos);
            _pPos += 1;
             // for user-id
            std::string _strFriendId(_pPos,_nUserIdLength);
           _pPos += _nUserIdLength;

            //char _strUserId[_nUserIdLength + 1];
            //memset(_strUserId, 0, sizeof(_strUserId));
            //memcpy(_strUserId, _pPos, _nUserIdLength);

            _arrFriends.Add(_strFriendId);
        }

        m_nUpdateFriendListCallback(_arrFriends);
    }
    else if(_nType == NEED_TO_PROCESS_USER_LIST_RES)
    {

        char* _pPos = (char*)data;
        _pPos += 2;
        // for need-to-process-user number
        int8_t _nFriendNum = *((int8_t*)_pPos);
        _pPos += 1;
        NDataStruct::DynArray<QByteArray> _arrFriends;
        for(int i = 0; i < _nFriendNum; i++)
        {
            // for user-id length
            int8_t _nUserIdLength = *((int8_t*)_pPos);
            _pPos += 1;
            // for user-id
            char _strUserId[_nUserIdLength + 1];
            memset(_strUserId, 0, sizeof(_strUserId));
            memcpy(_strUserId, _pPos, _nUserIdLength);
            _pPos += _nUserIdLength;
            _arrFriends.Add(QByteArray(_strUserId));
        }

        m_nNeedToProcessUserListCallback(_arrFriends);
    }
    // process send-chat-res
    else if(_nType == CHAT_RES)
    {
        char* _pPos = (char*)data;
        _pPos += 2;
        // for res value
        int8_t _nResValue = *((int8_t*)_pPos);
        m_nChatCallback(_nResValue);
    }
    else if(_nType ==SEND_PHOTO_RES)
    {
        char* _pPos = (char*)data;
        _pPos += 2;
        // for res value
        int8_t _nResValue = *((int8_t*)_pPos);
        std::cout<<__FILE__<<"  :" <<__LINE__<<"_nResValue= "<<(int)_nResValue<<std::endl<<std::flush;
        m_PhotoMessageCallback((int)_nResValue);
    }
    else if(_nType == NEW_MESSAGE_NOTIFICATION)
    {
        char* _pPos = (char*)data;
        _pPos += 2;
        // for user-id-length
        int8_t _nLen = *((int8_t*)_pPos);
        _pPos += 1;
        // for user-id
        char _strUserId[_nLen + 1];
        memset(_strUserId, 0, sizeof(_strUserId));
        memcpy(_strUserId, _pPos, _nLen);

        m_nNewMessageCallback(_strUserId);
    }
    else if(_nType == GET_NEED_TO_PROCESS_MESSAGES_RES)
    {
        char* _pPos = (char*)data;
        _pPos += 2;
        // for message-nums
        int16_t _nNetLen = *((int16_t*)_pPos);
        int16_t _nLen = networkToHost16(_nNetLen);
        _pPos += 2;

        // for every-message
        NDataStruct::DynArray<Message> _arrMessages;
        for(int i = 0; i < _nLen; i++)
        {
            // 1 byte for direction
            int8_t _nDirection = *((int8_t*)_pPos);
            _pPos += 1;
            // 2 byte for message-length
            int16_t _nMessNetLen = *((int16_t*)_pPos);
            int16_t _nMessLen = networkToHost16(_nMessNetLen);
            _pPos += 2;
            char _strMessage[_nMessLen + 1];
            memset(_strMessage, 0, sizeof(_strMessage));
            memcpy(_strMessage, _pPos, _nMessLen);
            _pPos += _nMessLen;
            // 8 byte for timestamp
            int64_t _nNetTs = *((int64_t*)_pPos);
            int64_t _nTs = networkToHost64(_nNetTs);
            TimeStamp _nTimeStamp(_nTs);
            _pPos += 8;
            Message _nMess;
            _nMess.m_bDirection = _nDirection == 1;
            _nMess.m_nContent = _strMessage;
            _nMess.m_nTimeStamp = _nTimeStamp;
            _arrMessages.Add(_nMess);
        }

        m_nGetNeedToProcessMessagesCallback(_arrMessages);
    }
    else if(_nType==GET_NEED_TO_PROCESS_PHOTO_MESSAGE_RES)
    {
        // pack and send to client
        // information format
        // 4 byte for length, thus we can send a message at most 2^32-1 byte
        // 2 byte for type, thus we can process 2^16-1 different types.
        // for get-need-to-process-photomessages-res
        // the content format
        // 2 byte for need-to-process-photomessages number
        // for every message
        // 1 byte for record its direction
        // 8 byte for timestamp
        // 1 byte for record its photo name,followed by photo name content
        // 2 byte for photomessage length, followed by photomessage content
std::cout<<__FILE__<<" : "<<__LINE__<<std::endl<<std::flush;
        char* _pPos = (char*)data;
        _pPos += 2;
 std::cout<<__FILE__<<" : "<<__LINE__<<std::endl<<std::flush;
        // for photomessage-nums
        int16_t _nNetLen = *((int16_t*)_pPos);
        int16_t _nLen = networkToHost16(_nNetLen);//photo message number
        _pPos += 2;

    std::cout<<__FILE__<<" : "<<__LINE__<<"photomessage number: "<<_nLen<<std::endl<<std::flush;

        // for every-message
        NDataStruct::DynArray<PhotoMessage> _arrPhotoMessages;
        for(int i = 0; i < _nLen; i++)
        {
            std::cout<<__FILE__<<" : "<<__LINE__<<std::endl<<std::flush;
            // 1 byte for direction
            int8_t _nDirection = *((int8_t*)_pPos);
            _pPos += 1;

            // 8 byte for timestamp
            int64_t _nNetTs = *((int64_t*)_pPos);
            int64_t _nTs = networkToHost64(_nNetTs);
            TimeStamp _nTimeStamp(_nTs);
            _pPos += 8;

            uint8_t _nImageNameLen=*((uint8_t*)_pPos);
            _pPos +=1;
            std::cout<<__FILE__<<" : "<<__LINE__<<std::endl<<std::flush;
            std::string ImageName(_pPos,_pPos+_nImageNameLen);
            _pPos +=_nImageNameLen;
std::cout<<__FILE__<<" : "<<__LINE__<<std::endl<<std::flush;
            // 4 byte for photomessage-length
            uint32_t _nPhotoMessNetLen = *((uint32_t*)_pPos);
            uint32_t _nPhotoMessLen = networkToHost32(_nPhotoMessNetLen);
            _pPos += 4;
            std::cout<<__FILE__<<" : "<<__LINE__<<std::endl<<std::flush;
            std::string strPhoto(_pPos,_nPhotoMessLen);
            std::cout<<__FILE__<<" : "<<__LINE__<<std::endl<<std::flush;
            _pPos += _nPhotoMessLen;

            //char _strMessage[_nMessLen + 1];
            //memset(_strMessage, 0, sizeof(_strMessage));
            //memcpy(_strMessage, _pPos, _nMessLen);
            //_pPos += _nMessLen;
std::cout<<__FILE__<<" : "<<__LINE__<<std::endl<<std::flush;
            PhotoMessage _nPhotoMess;
            _nPhotoMess.m_bDirection = _nDirection == 1;
            _nPhotoMess.m_nTimeStamp = _nTimeStamp;
            _nPhotoMess.m_photoName = ImageName;
            _nPhotoMess.m_photoMessage=strPhoto;
            std::cout<<__FILE__<<" : "<<__LINE__<<std::endl<<std::flush;
            _arrPhotoMessages.Add(_nPhotoMess);
            std::cout<<__FILE__<<" : "<<__LINE__<<std::endl<<std::flush;
        }

        std::cout<<__FILE__<<" : "<<__LINE__<<std::endl<<std::flush;
        std::cout<<__FILE__<<" : "<<__LINE__<<"  "<<_arrPhotoMessages.GetSize()<<std::endl<<std::flush;
        m_nGetNeedToProcessPhotoMessagesCallback(_arrPhotoMessages);
        std::cout<<__FILE__<<" : "<<__LINE__<<std::endl<<std::flush;
    }
    else if(_nType==UPDATE_MY_INFO_RES)
    {
        char* _pPos = (char*)data;
        _pPos += 2;
        int8_t studentIdLen=*reinterpret_cast<int8_t*>(_pPos);
        _pPos +=1;
        std::string studentid(_pPos,studentIdLen);
        std::cout<<"查询到学生id为："<<studentid<<std::endl<<std::flush;
        _pPos +=studentIdLen;

        int8_t nameLen=*reinterpret_cast<int8_t*>(_pPos);
        _pPos +=1;
        std::string name(_pPos,nameLen);
        _pPos +=nameLen;

        int8_t genderLen=*reinterpret_cast<int8_t*>(_pPos);
        _pPos +=1;
        std::string gender(_pPos,genderLen);
        _pPos +=genderLen;

        //int32_t NetHeadImgLen=*reinterpret_cast<int32_t*>(_pPos);
        //int32_t HostHeadImgLen=networkToHost32(NetHeadImgLen);
        //_pPos +=4;
        //QByteArray ByteHeadImg(_pPos,HostHeadImgLen);
        //QPixmap headImg;
        //headImg.loadFromData(ByteHeadImg);
        //_pPos +=HostHeadImgLen;

        int8_t schoolLen=*reinterpret_cast<int8_t*>(_pPos);
        _pPos +=1;
        std::string school(_pPos,schoolLen);
        //QPixmap pixmap;
        UserInfo latestMyInfo(studentid,name,gender,school);
        std::cout<<"即将执行更新用户个人信息回调函数"<<std::endl<<std::flush;
        m_UpdateMyInfoCallback(latestMyInfo);
    }
    else if(_nType==MODIFY_MY_INFO_RES)
    {
        char* _pPos = (char*)data;
        _pPos += 2;
        int8_t retNum=*reinterpret_cast<int8_t*>(_pPos);
        m_ModifyMyInfoCallbak(retNum);

    }

}

// send connecting request to server and start watching the event of readable
void MuduoClient::connect()
{
    m_nClient.connect();
}

// send fin to server to end connection
void MuduoClient::disconnect()
{
    m_nClient.disconnect();
}

// TcpClient本身不提供写如套接字接口
// 可能是写入消息本身有一个协议编码过程
// 原始信息－－协议编码－－编码后信息写入交给应用层处理
// 利用TcpConnection对象可写入传入信息

// the parameter is the origin message to write
// it should be coded before actually writed to server
/*void MuduoClient::write(const StringPiece& message)
{
    MutexLockGuard lock(m_nMutex);
    if (m_nConnection)
    {
      m_nCodec.send(
          get_pointer(m_nConnection),
          message);
    }
}*/

// the time of connection build and destroy
void MuduoClient::onConnection(
      const TcpConnectionPtr& conn)
{
    LOG_TRACE
        << conn->localAddress().toIpPort()
        << " -> "
        << conn->peerAddress().toIpPort()
        << " is "
        << (conn->connected() ? "UP" : "DOWN");

    MutexLockGuard lock(m_nMutex);
    if (conn->connected())
    {
        m_nConnection = conn;
    }
    else
    {
        m_nConnection.reset();
    }
}

// this is called when there have a completed message to process
// the parameter 2 is the message content.
void MuduoClient::onStringMessage(
      const TcpConnectionPtr&,
      const string& message,
      TimeStamp)
{
    printf(
        "<<< %s\n",
        message.c_str());
}
