
#include "main.h"
#include <QGuiApplication>
int main(int argc, char *argv[])

{
     QGuiApplication app(argc, argv);
    LOG_INFO
        << "pid = "
        << getpid();
    EventLoop loop;
    uint16_t port = static_cast<uint16_t>(atoi("20201"));
    InetAddress serverAddr(port);
    char _strHost[] = "127.0.0.1";
    char _strUser[] = "muduoclient";
    char _strPassword[] = "mysql";
    int max_connection=2;
  /*  MuduoServer server(
        &loop,
        serverAddr,
        _strHost,
        strlen(_strHost),
        _strUser,
        strlen(_strUser),
        _strPassword,
        strlen(_strPassword),=
            max_connection
                ); */
    MuduoServer server(&loop,
                       serverAddr,
                       _strHost,
                       strlen(_strHost),
                       _strUser,
                       strlen(_strUser),
                       _strPassword,
                       strlen(_strPassword),
                       2);


    server.setThreadNum(atoi("3"));
    server.start();
    loop.loop();
    int i = 0;
    i++;
}
