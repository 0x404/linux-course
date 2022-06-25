#include "chatorm.h"

ChatORM::ChatORM()
{

}

ChatORM::ChatORM(int senderId, int receiverId, QString content, QString time)
{
    this->senderId = senderId;
    this->receiverId = receiverId;
    this->content = content;
    this->time = time;
}

ChatORM::ChatORM(int senderId, QString content, QString time, QString info)
{
    this->senderId = senderId;
    this->content = content;
    this->time = time;
    this->info = info;
}



int ChatORM::getSenderId() const
{
    return senderId;
}

void ChatORM::setSenderId(int value)
{
    senderId = value;
}

int ChatORM::getReceiverId() const
{
    return receiverId;
}

void ChatORM::setReceiverId(int value)
{
    receiverId = value;
}

QString ChatORM::getContent() const
{
    return content;
}

void ChatORM::setContent(const QString &value)
{
    content = value;
}

QString ChatORM::getTime() const
{
    return time;
}

void ChatORM::setTime(const QString &value)
{
    time = value;
}

void ChatORM::setInfo(const QString &value)
{
    info = value;
}

QString ChatORM::getInfo() const
{
    return info;
}

