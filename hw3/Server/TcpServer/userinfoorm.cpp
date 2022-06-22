#include "userinfoorm.h"

UserInfoORM::UserInfoORM()
{

}

UserInfoORM::UserInfoORM(int id, QString username, QString motto, std::vector<int> friendList, QString otherinfo)
{
    this->id=id;
    this->username=username;
    this->motto=motto;
    this->friendList=friendList;
    this->otherinfo=otherinfo;
}

QString UserInfoORM::toString()
{
    return QString("{ id: "+ QString::number(id, 10) + ", username: "+username+", motto: "+motto+ ", friendNum: "+ QString::number(int(friendList.size() ), 10) + ", otherInfo: "+otherinfo+" }");
}


int UserInfoORM::getId() const
{
    return id;
}

void UserInfoORM::setId(int value)
{
    id = value;
}

QString UserInfoORM::getUsername() const
{
    return username;
}

void UserInfoORM::setUsername(const QString &value)
{
    username = value;
}

QString UserInfoORM::getMotto() const
{
    return motto;
}

void UserInfoORM::setMotto(const QString &value)
{
    motto = value;
}

QString UserInfoORM::getOtherinfo() const
{
    return otherinfo;
}

void UserInfoORM::setOtherinfo(const QString &value)
{
    otherinfo = value;
}

std::vector<int> UserInfoORM::getFriendList() const
{
    return friendList;
}

void UserInfoORM::setFriendList(const std::vector<int> &value)
{
    friendList = value;
}
