#include "requestorm.h"

RequestORM::RequestORM()
{

}

RequestORM::RequestORM(UserInfoORM *userSend, UserInfoORM *userReceive, int status, QString sendTime, QString receiveTime)
{
    this->userSend = userSend;
    this->userReceive = userReceive;
    this->status = status;
    this->sendTime = sendTime;
    this->receiveTime = receiveTime;
}

UserInfoORM *RequestORM::getUserSend() const
{
    return userSend;
}

void RequestORM::setUserSend(UserInfoORM *value)
{
    userSend = value;
}

UserInfoORM *RequestORM::getUserReceive() const
{
    return userReceive;
}

void RequestORM::setUserReceive(UserInfoORM *value)
{
    userReceive = value;
}

int RequestORM::getStatus() const
{
    return status;
}

void RequestORM::setStatus(int value)
{
    status = value;
}

QString RequestORM::getSendTime() const
{
    return sendTime;
}

void RequestORM::setSendTime(const QString &value)
{
    sendTime = value;
}

QString RequestORM::getReceiveTime() const
{
    return receiveTime;
}

void RequestORM::setReceiveTime(const QString &value)
{
    receiveTime = value;
}
