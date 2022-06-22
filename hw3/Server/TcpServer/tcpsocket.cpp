#include "tcpsocket.h"

TCPSocket::TCPSocket(QObject* parent)
{
    connect(this, SIGNAL(readyRead()), this, SLOT(receiveData()));
    connect(this, SIGNAL(disconnected()), this, SLOT(disconnectFromHost()));
}

/**************************************************************************
 * @author: 曾群鸿
 * @Date: 2021-8-26
 * @Description: 当一个socket接受到信息，释放一个messageComing信号给服务器，并传递信息
 * @Parameters:
 **************************************************************************/
void TCPSocket::receiveData()
{
    QString buffer = readAll();
    emit messageComing(buffer, socketDescriptor());
}

/**************************************************************************
 * @author: 曾群鸿
 * @Date: 2021-8-26
 * @Description: 当一个socket断开连接，释放一个clientDisconnect信号给服务器，并传递标识符
 * @Parameters:
 **************************************************************************/
void TCPSocket::disconnectFromHost()
{
    emit clientDisconnect(descriptor);
}

/**************************************************************************
 * @author: 曾群鸿
 * @Date: 2021-8-26
 * @Description: 获取当前socket的标识符
 * @Parameters:
 **************************************************************************/
qintptr TCPSocket::getDescriptor()
{
    return descriptor;
}

/**************************************************************************
 * @author: 曾群鸿
 * @Date: 2021-8-26
 * @Description: 初始化socket
 * @Parameters: qintptr 标识符
 **************************************************************************/
void TCPSocket::init(qintptr x)
{
    descriptor = x;
}
