#ifndef REQUESTORM_H
#define REQUESTORM_H
#include "userinfoorm.h"

class RequestORM
{
public:
    RequestORM();
    RequestORM(UserInfoORM* userSend, UserInfoORM* userReceive, int status, QString sendTime, QString receiveTime);

    UserInfoORM *getUserSend() const;
    void setUserSend(UserInfoORM *value);

    UserInfoORM *getUserReceive() const;
    void setUserReceive(UserInfoORM *value);

    int getStatus() const;
    void setStatus(int value);

    QString getSendTime() const;
    void setSendTime(const QString &value);

    QString getReceiveTime() const;
    void setReceiveTime(const QString &value);

private:
    UserInfoORM* userSend;          //发送申请的用户id
    UserInfoORM* userReceive;       //接受申请的用户id
    int status;                     //1表示userReceive 以及 接受了 userSend的好友申请； 0表示userSend 发送了好友申请向UserSend 但是UserSend还没有接受
    QString sendTime;
    QString receiveTime;
};

#endif // REQUESTORM_H
