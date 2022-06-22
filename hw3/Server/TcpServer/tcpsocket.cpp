#include "tcpsocket.h"

TCPSocket::TCPSocket(QObject* parent)
{
    connect(this, SIGNAL(readyRead()), this, SLOT(receiveData()));
    connect(this, SIGNAL(disconnected()), this, SLOT(disconnectFromHost()));
}

void TCPSocket::receiveData()
{
    QString buffer = readAll();
    emit messageComing(buffer, socketDescriptor());
}


void TCPSocket::disconnectFromHost()
{
    emit clientDisconnect(descriptor);
}

qintptr TCPSocket::getDescriptor()
{
    return descriptor;
}

void TCPSocket::init(qintptr x)
{
    descriptor = x;
}
