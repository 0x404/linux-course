#ifndef TCPSERVER_H
#define TCPSERVER_H
#include "tcpsocket.h"
#include "datapackage.h"
#include "dataanalyst.h"
#include "databasemanager.h"

#include <QTcpServer>
#include <QList>
#include <list>
#include <map>

class TCPServer : public QTcpServer
{
    Q_OBJECT
public:
    TCPServer(QObject *parent = nullptr);
signals:
    void getRegister();
public slots:
    void messageComing(QString, qintptr);  //当有新消息被发送到服务器
    void clientDisconnect(qintptr);             //当有客服端离开服务器
protected:
    void incomingConnection(qintptr socketDescriptor);
    QList<TCPSocket*> clientLists;  // 当前连接本服务器的所有客服端链表
    QList<std::pair<int, TCPSocket*>> onlineUser;  //当前在线且已经注册的用户信息
    std::map<int, TCPSocket*> idToClient;   // 将用户ID映射到对应的socket

    void sendToClient(DataPackage*, qintptr); //向一个指定socket id的客服端发送数据包
    void sentToAllClient(DataPackage*);       //向所有连接本服务器的客服端发送数据包
    void sendToUser(DataPackage*, int);       //向指定用户id，且在线的用户客户端发送数据包
    void sendToAllUser(DataPackage*);         //向当前在线的所有用户客户端发送数据包

    bool userIsOnline(QString);     //通过用户名判断用户是否上线
    bool userIsOnline(int);         //通过用户ID判断用户是否上线
    void userOffLine(QString);      //通过用户名，强制用户下线
    void userOffLine(int);          //通过用户ID，强制用户下线
    qintptr descriptorOfUser(QString);  //通过用户ID，获取其socket的ID
    qintptr descriptorOfUser(int);  //通过用户ID，获取其socket的ID

    void requestLogin(DataPackage*, qintptr);        // 客户端请求登录
    void requestRegister(DataPackage*, qintptr);     // 客户端请求注册
    void requestSendMessage(DataPackage*, qintptr);   // 客户端请求发送信息给其他用户，目前仅支持向在线的用户发送信息
    void requestChatHistory(DataPackage*, qintptr);   // 客户端请求好友聊天记录
    void requestMyInfo(DataPackage*, qintptr);        // 客户端请求自身信息，用于初始化客户端界面
    void requestFriendInfo(DataPackage*, qintptr);    // 客户端请求其好友信息，用于初始化好友界面
    void requestAddFriend(DataPackage*, qintptr);     // 客户端请求添加好友
    void requestFriendInviteAccepted(DataPackage*, qintptr);   // 客户端接收好友邀请
    void requestFriendInviteRejected(DataPackage*, qintptr);   // 客户端拒绝好友邀请 a向b发送邀请 b拒绝
    void requestFriendInviteList(DataPackage*, qintptr);       // 客户端请求好友邀请列表
    void requestDeleteFriend(DataPackage*, qintptr);          // 客户端请求删除好友
    void requestUpdateMotto(DataPackage*, qintptr);           // 客户端请求更新签名
    void requestGroupMessage(DataPackage*, qintptr);          // 客户端发送群聊消息
    void requestAllUser(DataPackage*, qintptr);               // 客户端请求所有用户
    void requestChangeProfile(DataPackage*, qintptr);         // 客户端请求更换头像
    void requestGroupChatHistory(DataPackage*, qintptr);      // 客户端请求群聊聊天记录
    void requestSendFile(DataPackage*, qintptr);              // 客户端请求发送文件
};

#endif // TCPSERVER_H
