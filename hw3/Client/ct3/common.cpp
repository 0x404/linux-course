#include "common.h"

QTcpSocket* common::socket = nullptr;
QString common::userName = "";
QString common::profile = "";
common::common()
{

}

QTcpSocket* common::getSocket()
{
    if (socket == nullptr)
        socket = new QTcpSocket();
    return socket;
}
