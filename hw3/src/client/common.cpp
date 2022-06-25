#include "common.h"

QTcpSocket* common::socket = nullptr;
QString common::userName = "";
QString common::profile = "";
common::common()
{

}

QTcpSocket* common::getSocket()
{
    // socket单例设计，保证唯一
    if (socket == nullptr)
        socket = new QTcpSocket();
    return socket;
}
