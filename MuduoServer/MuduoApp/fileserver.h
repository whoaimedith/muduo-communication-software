#ifndef FILESERVER_H
#define FILESERVER_H

#include <QObject>
#include  "../MyMuduo/Tcp/lib.h"

class FileServer
{

public:
    FileServer(    EventLoop* loop,
                   const InetAddress& listenAddr);
public:
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

private:
    TcpServer m_nServer;

};

#endif // FILESERVER_H
