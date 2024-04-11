#include "fileserver.h"

FileServer::FileServer(    EventLoop* loop,
                           const InetAddress& listenAddr)
    :m_nServer(loop,listenAddr,"fileServer")

{
    m_nServer.setConnectionCallback(std::bind(&FileServer::onConnection,this,_1));
}

void FileServer::onConnection(const TcpConnectionPtr &conn)
{
    LOG_TRACE
        << conn->localAddress().toIpPort()
        << " -> "
        << conn->peerAddress().toIpPort()
        << " is "
        << (conn->connected() ? "UP" : "DOWN");

    if (conn->connected())
    {
        // 让调用线程在其自己的特定的动态对象里
        // 存储由其负责的所有TcpConnection对象
        LocalConnections::instance().insert(conn);

        // MutexLockGuard lock(m_nMutex);
        // m_mapConnections.insert(std::pair());
    }
    else
    {
        LocalConnections::instance().erase(conn);

        // MutexLockGuard lock(m_nMutex);
        // ConnectionMapping m_mapConnections;
    }

}
