#ifndef COMMON_H
#define COMMON_H

#include <QTcpSocket>

class common    // socket 单例类
{
public:
    common();
    static QTcpSocket* getSocket();
    static QString userName;
    static QString profile;
private:
    static QTcpSocket *socket;
};

#endif // COMMON_H
