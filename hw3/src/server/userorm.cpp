#include "userorm.h"

UserORM::UserORM()
{

}

UserORM::UserORM(int id, QString username, QString psw)
{
    this->id = id;
    this->username=username;
    this->password = psw;
}

int UserORM::getQId()
{
    return id;
}

void UserORM::setQId(int id)
{
    this->id=id;
}


QString UserORM::getUserName()
{
    return username;
}

void UserORM::setUserName(QString username)
{
    this->username=username;
}

QString UserORM::getPassword()
{
    return password;
}

void UserORM::setPassword(QString psw)
{
    this->password=psw;
}
