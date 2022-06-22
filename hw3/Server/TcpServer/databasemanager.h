#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H
#include <vector>
#include "database.h"

class DataBaseManager
{
public:
    DataBaseManager();
    static DataBaseManager *getManager();

    bool sqlValidLogin(QString, QString); // 判断账号和密码是否匹配，应该由数据库类实现，这里作为一个测试函数
    bool sqlValidUserName(QString);     //判断用户名是否可用
    bool sqlRegister(QString, QString); //实现注册，注册成功返回true
    int sqlGetIdFromUserName(QString);  //返回用户名对应的userID
    QString sqlGetUserNameFromId(int);
    std::vector<QString> sqlGetInfo(QString);   //返回结果格式为{userName, state}，通过用户名获取其签名等信息
    std::vector<QString> sqlGetFriendList(QString); //通过用户名，返回其好友信息表
    std::vector<QString> sqlGetFriendInviteList(QString); //通过用户名，返回添加好友信息
    std::vector<QString> sqlGetChatHistory(QString, QString);   //返回聊天记录
    std::vector<QString> sqlGetGroupChatHistory();
    bool sqlIsFriend(QString, QString);         //检查两个用户名之前是否是好友关系
    bool sqlAddFriend(QString, QString);        //将两个好友之间添加好友
    bool sqlRejectAddFriend(QString, QString);  //拒绝好友申请
    bool sqlInsertAddFriend(QString, QString);  //插入好友申请表
    bool sqlInsertChatHistory(QString, QString, QString);   //插入聊天记录
    bool sqlDeleteFriend(QString, QString);     //删除好友
    bool sqlUpdateMotto(QString, QString);         //更新签名
    bool sqlUpdateProfile(QString, QString);        //更新头像
    bool sqlUpdateGroupChatHistory(QString, QString, QString);

private:
    static DataBaseManager *manager;
};

#endif // DATABASEMANAGER_H



