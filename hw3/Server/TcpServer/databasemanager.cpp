#include "databasemanager.h"


DataBaseManager *DataBaseManager::manager = nullptr;

DataBaseManager::DataBaseManager()
{

}

DataBaseManager *DataBaseManager::getManager()
{
    if (manager == nullptr) manager = new DataBaseManager();
    return manager;
}


bool DataBaseManager::sqlValidUserName(QString userName)
{
    DataBase *db = DataBase::getInstance();
    UserORM *res = db->queryUserByName(userName);
    if (res == nullptr) return false;
    return true;
}

bool DataBaseManager::sqlValidLogin(QString userName, QString passwd)
{
    DataBase *db = DataBase::getInstance();
    if (!sqlValidUserName(userName))
    {
        return false;
    }
    UserORM *res = db->queryUserByName(userName);
    if (res->getUserName() == userName && res->getPassword() == passwd)
        return true;
    return false;
}

bool DataBaseManager::sqlRegister(QString userName, QString passwd)
{
    DataBase *db = DataBase::getInstance();
    if (sqlValidUserName(userName)) return false;
    return db->registerUser(userName, passwd);
}

int DataBaseManager::sqlGetIdFromUserName(QString userName)
{
    DataBase *db = DataBase::getInstance();
    if (!sqlValidUserName(userName)) return -1;
    UserORM *res = db->queryUserByName(userName);
    return res->getQId();
}

QString DataBaseManager::sqlGetUserNameFromId(int id)
{
      UserORM *res = DataBase::getInstance()->queryUserById(id);
      return res->getUserName();
}

 std::vector<QString> DataBaseManager::sqlGetInfo(QString userName)
 {
    DataBase *db = DataBase::getInstance();
    UserInfoORM *res = db->queryUserInfoByName(userName);
    return std::vector<QString>{res->getUsername(), res->getMotto(), !res->getOtherinfo().length() ? "11" : res->getOtherinfo()};
 }

 std::vector<QString> DataBaseManager::sqlGetFriendList(QString userName)
 {
    DataBase *db = DataBase::getInstance();
    UserInfoORM *userInfo = db->queryUserInfoByName(userName);
    std::vector<int> ls = userInfo->getFriendList();
    std::vector<QString> res;
    for (auto x : ls)
    {
       UserORM *user = db->queryUserById(x);
       res.push_back(user->getUserName());
    }
    return res;
 }

 bool DataBaseManager::sqlIsFriend(QString userName1, QString userName2)
 {
     DataBase *db = DataBase::getInstance();
     return db->isExistInRelationShipTable(sqlGetIdFromUserName(userName1), sqlGetIdFromUserName(userName2));
 }


 bool DataBaseManager::sqlAddFriend(QString userName1, QString userName2)
 {
     int userId1 = sqlGetIdFromUserName(userName1);
     int userId2 = sqlGetIdFromUserName(userName2);
     return DataBase::getInstance()->admitBecomeFriend(userId1, userId2);
 }

 bool DataBaseManager::sqlRejectAddFriend(QString userName1, QString userName2)
 {
    int userId1 = sqlGetIdFromUserName(userName1);
    int userId2 = sqlGetIdFromUserName(userName2);
    return DataBase::getInstance()->removeFriendRequest(userId1, userId2);
 }

 bool DataBaseManager::sqlInsertAddFriend(QString userName1, QString userName2)
 {
    int userId1 = sqlGetIdFromUserName(userName1);
    int userId2 = sqlGetIdFromUserName(userName2);
    return DataBase::getInstance()->sendRequestId1AddId2(userId1, userId2);
 }

bool DataBaseManager::sqlDeleteFriend(QString userName1, QString userName2)
{
    int userId1 = sqlGetIdFromUserName(userName1);
    int userId2 = sqlGetIdFromUserName(userName2);
    return DataBase::getInstance()->deleteRelationship(userId1, userId2);
}

 bool DataBaseManager::sqlInsertChatHistory(QString sendName, QString receiverName, QString content)
 {
     int userId1 = sqlGetIdFromUserName(sendName);
     int userId2 = sqlGetIdFromUserName(receiverName);
     return DataBase::getInstance()->insertIntoChatHistory(userId1, userId2, content);

 }

 bool DataBaseManager::sqlUpdateGroupChatHistory(QString sendName, QString content, QString info)
 {
    int userId = sqlGetIdFromUserName(sendName);
    return DataBase::getInstance()->insertIntoGroupChatHistory(userId, content, info);
 }

 std::vector<QString> DataBaseManager::sqlGetChatHistory(QString userName1, QString userName2)
 {
     int userId1 = sqlGetIdFromUserName(userName1);
     int userId2 = sqlGetIdFromUserName(userName2);
     std::vector<ChatORM*> records = DataBase::getInstance()->getALLChatHistory(userId1, userId2);
     std::vector<QString> res;
     for (int i = records.size() - 1; i >= 0; --i)
     {
         ChatORM *x = records[i];
         res.push_back(sqlGetUserNameFromId(x->getSenderId()));
         res.push_back(sqlGetUserNameFromId(x->getReceiverId()));
         res.push_back(x->getContent());
         res.push_back(x->getTime());
     }
     return res;
 }

 std::vector<QString> DataBaseManager::sqlGetGroupChatHistory()
 {
    std::vector<ChatORM*> records = DataBase::getInstance()->getAllGroupChatHistory();

    std::vector<QString> res;
    for (int i = records.size() - 1; i >= 0; --i)
    {
        ChatORM *x = records[i];
        res.push_back(sqlGetUserNameFromId(x->getSenderId()));
        res.push_back(x->getContent());
        res.push_back(x->getTime());
        res.push_back(x->getInfo());
    }
    return res;
 }


 std::vector<QString> DataBaseManager::sqlGetFriendInviteList(QString userName)
 {
     int userId = sqlGetIdFromUserName(userName);
     std::vector<RequestORM*> req = DataBase::getInstance()->getALLRequestUserId_ById(userId);

     std::vector<QString> res;
     for (auto x : req)
     {
        if (x->getStatus() == 0)
        {
            res.push_back(x->getUserSend()->getUsername());
            res.push_back(x->getSendTime());
        }
     }
     return res;
 }

 bool DataBaseManager::sqlUpdateMotto(QString userName, QString motto)
 {
    DataBase::getInstance()->setMottoByName(userName, motto);
 }

 bool DataBaseManager::sqlUpdateProfile(QString userName, QString picId)
 {
    DataBase::getInstance()->updateOhterInfoByName(userName, picId);

 }

