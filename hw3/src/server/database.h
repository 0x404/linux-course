#ifndef DATABASE_H
#define DATABASE_H
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QVariantList>
#include <QtDebug>
#include <QDir>

#include "userorm.h"            //导入UserORM类
#include "userinfoorm.h"        //导入UserInfoORM类
#include "requestorm.h"         //导入request类
#include "chatorm.h"            //导入chatorm类

class DataBase
{
public:

    ~DataBase();
    static DataBase* getInstance(); //单例模式，返回实例

    bool registerUser(QString username,QString psw);                //通过用户名与密码注册
    UserORM* queryUserByName(QString username);                     //没有查询到，返回空指针，否则返回user对象
    UserORM* queryUserById(int id);                                 //没有查询到，返回空指针
    bool deleteUserByName(QString username);                        //通过用户名删除
    bool updateUserPasswordByName(QString username, QString psw);   //更新username对应的密码

    UserInfoORM* queryUserInfoByName(QString username);             //返回用户具体信息（包括所有好友）
    bool setMottoByName(QString name, QString motto);               //设置个性签名
    QString getMottoByName(QString name);                           //获取个性签名
    QString getOtherInfoByName(QString name);                       //获取其他信息
    bool updateOhterInfoByName(QString name, QString info);         //更新其他信息

    bool removeFriendRequest(int useid1, int useid2);               //删除useid1向useid2添加好友的信息
    bool addFriendById(int useid1,int useid2);                      //添加好友关系
    std::vector<int> queryFriendByID(int useId);                    //查询所有好友，返回对应的Id
    bool deleteRelationship(int useid1,int useid2);                 //删除好友关系
    bool isExistInRelationShipTable(int id1, int id2);              //判断relationship中是否已经存在当前记录


    bool sendRequestId1AddId2(int id1, int id2);                    //id1添加id2，注意返回false表示两人已是好友关系无需添加
    /*
     * @instance: admitBecomeFriend(10, 11);    11接受10的申请
     * */
    bool admitBecomeFriend(int id1, int id2);                       //id2允许添加id1为好友，注意顺序！！！！！！！！！！！！！！！！
    std::vector<RequestORM*> getALLRequestUserId_ById(int id);              //查看id对应用户所有好友申请信息


    bool insertIntoChatHistory(int senderId, int receiverId, QString content); //插入聊天记录
    std::vector<ChatORM*> getALLChatHistory(int id1, int id2);          //按插入的时间升序输出
    std::vector<ChatORM*> getAllGroupChatHistory();
    bool insertIntoGroupChatHistory(int senderId, QString content, QString info);

private:
    DataBase(); //单例模式构造器私有化

    QSqlDatabase db;
    static DataBase* dataBase;

    bool createOrIsExistUserTable();                            //建立user表
    bool createOrIsExistUserInfoTable();                        //建立userinfo表
    bool createOrIsExisRelationshipTable();                     //建立relationship表
    bool createOrIsExistRequestTable();                         //建立request表
    bool createOrIsExistChatHistoryTable();                     //建立chathistory表
    bool insertIntoUserInfoByName( QString motto);              //在user表注册时，同时将用户插入到userInfo之中
    bool createOrIsExistGroupChatHistory();                     //建立groupChatHistory

    static const QString CREATE_USER_SQL_STATEMENE;         //建立user表语句
    static const QString CREATE_USERINFO_SQL_STATEMENT;     //建立userinfo表语句
    static const QString CREATE_RELATIONSHIP_SQL_STATEMENE; //建立relationship表语句
    static const QString CREATE_REQUEST_SQL_STATEMENT;      //建立好友申请信息表
    static const QString CREATE_CHATHISTORY_SQL_STATEMENT;  //建立chatHistory表语句
    static const QString CREATE_GROUPCHATHISTORY_SQL_STATEMENT; //建立groupChatHistory表语句
};

#endif // DATABASE_H
