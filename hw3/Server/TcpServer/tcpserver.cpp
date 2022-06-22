#include "tcpserver.h"
#include <QDebug>

TCPServer::TCPServer(QObject *parent) : QTcpServer (parent)
{
    listen(QHostAddress::Any, 5500);
    if (isListening()) qDebug() << "isListening...";
}


/**************************************************************************
 * @author: 曾群鸿
 * @Date: 2021-8-26
 * @Description: 当有一个新的socket连接到服务器上，加入到socket队列中，并监听
 * @Parameters: qintptr 新加入的socket的标识符
 **************************************************************************/
void TCPServer::incomingConnection(qintptr socketDescriptor)    //当有一个新的socket连接到服务器
{
    TCPSocket* socket = new TCPSocket();
    socket->setSocketDescriptor(socketDescriptor);
    socket->init(socketDescriptor);
    qDebug() << "a new socket connected! ID: " << socketDescriptor;

    //绑定该socket的信号和槽函数
    connect(socket, SIGNAL(messageComing(QString, qintptr)), this, SLOT(messageComing(QString, qintptr)));
    connect(socket, SIGNAL(clientDisconnect(qintptr)), this, SLOT(clientDisconnect(qintptr)));

    clientLists.append(socket); //加入当前socket列表
}


/**************************************************************************
 * @author: 曾群鸿
 * @Date: 2021-8-26
 * @Description: 当监听到一个socket发送数据包到服务器上，对数据包进行处理
 * @Parameters: QString socket发送到服务器的信息  qintptr 信息来自于哪个socket
 **************************************************************************/
void TCPServer::messageComing(QString s, qintptr id)    // 服务器接受到标识符为id的socket发送的信息
{
    qDebug() << "Server get " << s;

    DataAnalyst transfer;
    DataPackage* pack = transfer.QStringToPackage(s);   // 将信息转成数据包


    if (!pack->isCompleted())   // 检查数据包完整性
    {
        sendToClient(transfer.errorPackage("formatError"), id);
    }

    if (pack->opt == transfer.getOptId("login"))    // 请求登录
    {
        // 登录需要两个参数
        if (pack->length != 2)
        {
            sendToClient(transfer.errorPackage("loginError"), id);
        }
        else requestLogin(pack, id);
    }

    if (pack->opt == transfer.getOptId("register")) // 请求注册
    {
        //注册需要两个参数
        if (pack->length != 2)
        {
            sendToClient(transfer.errorPackage("registerError"), id);
        }
        else requestRegister(pack, id);
    }

    if (pack->opt == transfer.getOptId("message"))  // 请求发送消息
    {
        //发送消息需要四个参数
        if (pack->length != 3)
        {
            sendToClient(transfer.errorPackage("messageError"), id);
        }
        else requestSendMessage(pack, id);
    }

    if (pack->opt == transfer.getOptId("requestMyInfo"))    // 请求获取个人信息
    {
        //获取信息需要一个参数
        if (pack->length != 1)
        {
            sendToClient(transfer.errorPackage("requestMyInfoError"), id);
        }
        else requestMyInfo(pack, id);
    }

    if (pack->opt == transfer.getOptId("requestFriendInfo"))    //  请求获取好友信息
    {
        //获取好友信息需要一个参数
        if (pack->length != 1)
        {
            sendToClient(transfer.errorPackage("requestFriendInfoError"), id);
        }
        else requestFriendInfo(pack, id);
    }

    if (pack->opt == transfer.getOptId("requestAddFriend"))     // 请求添加好友
    {
        //添加好友需要两个参数
        if (pack->length != 2)
        {
            sendToClient(transfer.errorPackage("requestAddFriendError"), id);
        }
        else requestAddFriend(pack, id);
    }

    if (pack->opt == transfer.getOptId("friendInviteAccepted"))
    {
        if (pack->length != 2)
        {
            sendToClient(transfer.errorPackage("formatError"), id);
        }
        else requestFriendInviteAccepted(pack, id);
    }

    if (pack->opt == transfer.getOptId("friendInviteRejected"))
    {
        if (pack->length != 2)
        {
            sendToClient(transfer.errorPackage("formatError"), id);
        }
        else requestFriendInviteRejected(pack, id);
    }

    if (pack->opt == transfer.getOptId("requestFriendInviteList"))
    {
        if (pack->length != 1)
        {
            sendToClient(transfer.errorPackage("formatError"), id);
        }
        else requestFriendInviteList(pack, id);
    }

    if (pack->opt == transfer.getOptId("requestChatHistory"))
    {
        if (pack->length != 2)
        {
            sendToClient(transfer.errorPackage("formatError"), id);
        }
        else requestChatHistory(pack, id);
    }

    if (pack->opt == transfer.getOptId("deleteFriend"))
    {
        if (pack->length != 2)
        {
            sendToClient(transfer.errorPackage("formatError"), id);
        }
        else requestDeleteFriend(pack, id);
    }

    if (pack->opt == transfer.getOptId("requestUpdateMotto"))
    {
        if (pack->length != 2)
        {
            sendToClient(transfer.errorPackage("formatError"), id);
        }
        else requestUpdateMotto(pack, id);
    }

    if (pack->opt == transfer.getOptId("requestAllUser"))
    {
        requestAllUser(pack, id);
    }

    if (pack->opt == transfer.getOptId("groupMessage"))
    {
        requestGroupMessage(pack, id);
    }

    if (pack->opt == transfer.getOptId("requestChangeProfile"))
    {
        requestChangeProfile(pack, id);
    }

    if (pack->opt == transfer.getOptId("requestGroupChatHistory"))
    {
        requestGroupChatHistory(pack, id);
    }
}


/**************************************************************************
 * @author: 曾群鸿
 * @Date: 2021-8-27
 * @Description: 客户端请求登录
 * @Parameters: DataPackege* socket发送的数据包  qintptr 该数据包来自哪个socket
 **************************************************************************/
void TCPServer::requestLogin(DataPackage *pack, qintptr id)
{
    DataAnalyst transfer;

    if (DataBaseManager::getManager()->sqlValidLogin(pack->parameters[0], pack->parameters[1]))
    {
        int userId = DataBaseManager::getManager()->sqlGetIdFromUserName(pack->parameters[0]);

        for (int i = 0; i < clientLists.size(); ++i)    // 向onlineUser中添加一个当前登录的节点
        {
            TCPSocket* now = clientLists.at(i);
            if (now->socketDescriptor() == id)
            {
                onlineUser.append(std::make_pair(userId, now));
                break;
            }
        }
        for (int i = 0; i < onlineUser.size(); ++i)
        {
            if (onlineUser[i].first != userId)
            {
                int uid = onlineUser[i].first;
                QString uname = DataBaseManager().getManager()->sqlGetUserNameFromId(uid);
                DataAnalyst tr;
                requestFriendInfo(tr.optToPackage("requestFriendInfo", {uname}), onlineUser[i].second->getDescriptor());
            }
        }
        sendToClient(transfer.acceptPackage("loginAccepted"), id);  // 返回登录成功给客户端
    }
    else
    {
        sendToClient(transfer.errorPackage("loginError"), id);      // 返回登录失败给客户端
    }
}


/**************************************************************************
 * @author: 曾群鸿
 * @Date: 2021-8-27
 * @Description: 客户端请求注册
 * @Parameters: DataPackege* socket发送的数据包  qintptr 该数据包来自哪个socket
 **************************************************************************/
void TCPServer::requestRegister(DataPackage *pack, qintptr id)
{
    DataAnalyst transfer;

    if (DataBaseManager::getManager()->sqlValidUserName(pack->parameters[0]))
    {
        sendToClient(transfer.errorPackage("registerError"), id);
        return;
    }
    if (DataBaseManager::getManager()->sqlRegister(pack->parameters[0], pack->parameters[1]))
    {
        sendToClient(transfer.acceptPackage("registerAccepted"), id);
        // pack->paramters[0] [1]
        return;
    }
    sendToClient(transfer.errorPackage("registerError"), id);
    emit getRegister();
    return;
}


/**************************************************************************
 * @author: 曾群鸿
 * @Date: 2021-8-27
 * @Description: 客户端请求获取其个人信息
 * @Parameters: DataPackege* socket发送的数据包  qintptr 该数据包来自哪个socket
 **************************************************************************/
void TCPServer::requestMyInfo(DataPackage *pack, qintptr id)
{
    DataAnalyst transfer;

    std::vector<QString> queryRes = DataBaseManager::getManager()->sqlGetInfo(pack->parameters[0]);
    sendToClient(transfer.optToPackage("receiveMyInfo", queryRes), id);
}

void TCPServer::requestUpdateMotto(DataPackage *pack, qintptr id)
{
    DataBaseManager::getManager()->sqlUpdateMotto(pack->parameters[0], pack->parameters[1]);
}


/**************************************************************************
 * @author: 曾群鸿
 * @Date: 2021-8-27
 * @Description: 客户端请求获取好友信息
 * @Parameters: DataPackege* socket发送的数据包  qintptr 该数据包来自哪个socket
 **************************************************************************/
void TCPServer::requestFriendInfo(DataPackage *pack, qintptr id)
{
    DataAnalyst transfer;
    std::vector<QString> friendList = DataBaseManager::getManager()->sqlGetFriendList(pack->parameters[0]);
    std::vector<QString> res;
    for (std::size_t i = 0; i < friendList.size(); ++i)
    {
        std::vector<QString> queryRes = DataBaseManager::getManager()->sqlGetInfo(friendList[i]);
        res.push_back(queryRes[0]);
        res.push_back(queryRes[2]);
        if (userIsOnline(queryRes[0])) res.push_back("1");
        else res.push_back("0");
//        res.push_back(queryRes[1]);
    }
    sendToClient(transfer.optToPackage("receiveFriendInfo", res), id);
}


/**************************************************************************
 * @author: 曾群鸿
 * @Date: 2021-8-27
 * @Description: 客户端请求发送消息
 * @Parameters: DataPackege* socket发送的数据包  qintptr 该数据包来自哪个socket
 **************************************************************************/
void TCPServer::requestSendMessage(DataPackage *pack, qintptr id)   //目前仅支持向在线的用户发送消息
{
    DataAnalyst transfer;
    QString senderName = pack->parameters[0];
    QString targetName = pack->parameters[1];
    QString content = pack->parameters[2];
    if (userIsOnline(targetName))
    {
        int targetId = DataBaseManager::getManager()->sqlGetIdFromUserName(targetName);
        sendToUser(transfer.optToPackage("message", {senderName, targetName, content}), targetId);
    }
    DataBaseManager::getManager()->sqlInsertChatHistory(senderName, targetName, content);
}


/**************************************************************************
 * @author: 曾群鸿
 * @Date: 2021-8-28
 * @Description: 客户端请求获取聊天记录
 * @Parameters: DataPackege* socket发送的数据包  qintptr 该数据包来自哪个socket
 **************************************************************************/
void TCPServer::requestChatHistory(DataPackage *pack, qintptr id)
{
    DataAnalyst transfer;
    QString userName1 = pack->parameters[0];
    QString userName2 = pack->parameters[1];
    std::vector<QString> res = DataBaseManager::getManager()->sqlGetChatHistory(userName1, userName2);
    sendToClient(transfer.optToPackage("receiveChatHistory", res), id);
}

void TCPServer::requestGroupChatHistory(DataPackage *pack, qintptr id)
{
    std::vector<QString> res = DataBaseManager::getManager()->sqlGetGroupChatHistory();
    DataAnalyst tr;
    sendToClient(tr.optToPackage("receiveGroupChatHistory", res), id);

}


/**************************************************************************
 * @author: 曾群鸿
 * @Date: 2021-8-28
 * @Description: 客户端请求添加好友
 * @Parameters: DataPackege* socket发送的数据包  qintptr 该数据包来自哪个socket
 **************************************************************************/
void TCPServer::requestAddFriend(DataPackage *pack, qintptr id)
{
    DataAnalyst transfer;
    bool flag = true;
    if (!DataBaseManager::getManager()->sqlValidUserName(pack->parameters[0])) flag = false;
    if (!DataBaseManager::getManager()->sqlValidUserName(pack->parameters[1])) flag = false;
    if (!flag)
    {
        sendToClient(transfer.errorPackage("requestAddFriendError"), id);
        return;
    }
    if (DataBaseManager::getManager()->sqlIsFriend(pack->parameters[0], pack->parameters[1]))
    {
        sendToClient(transfer.errorPackage("requestAddFriendError"), id);
        return;
    }

    QString userName1 = pack->parameters[0];
    QString userName2 = pack->parameters[1];
//    int userId1 = DataBaseManager::getManager()->sqlGetIdFromUserName(userName1);
    int userId2 = DataBaseManager::getManager()->sqlGetIdFromUserName(userName2);
    if (userIsOnline(userId2))
    {
        DataBaseManager::getManager()->sqlInsertAddFriend(userName1, userName2);
        sendToUser(transfer.optToPackage("receiveFriendInvite", {userName1, userName2}), userId2);
    }
    else
    {
        // 向数据库保存添加好友请求信息
        DataBaseManager::getManager()->sqlInsertAddFriend(userName1, userName2);
    }
}


/**************************************************************************
 * @author: 曾群鸿
 * @Date: 2021-8-28
 * @Description: 客户端通过好友请求，向服务器发送反馈
 * @Parameters: DataPackege* socket发送的数据包  qintptr 该数据包来自哪个socket
 **************************************************************************/
void TCPServer::requestFriendInviteAccepted(DataPackage *pack, qintptr id)
{
    //好友请求通过，向数据库写入数据，向在线方重新发送好友数据包，用于更新UI界面
    DataAnalyst transfer;
    DataBaseManager::getManager()->sqlAddFriend(pack->parameters[0], pack->parameters[1]);
    QString userName1 = pack->parameters[0];
    QString userName2 = pack->parameters[1];

    if (userIsOnline(userName1))
    {
        requestFriendInfo(transfer.optToPackage("requestFriendInfo", {userName1}), descriptorOfUser(userName1));
    }

    if (userIsOnline(userName2))
    {
        requestFriendInfo(transfer.optToPackage("requestFriendInfo", {userName2}), descriptorOfUser(userName2));
    }
}

/**************************************************************************
 * @author: 曾群鸿
 * @Date: 2021-8-29
 * @Description: 客户端拒绝好友邀请
 * @Parameters: DataPackege* socket发送的数据包  qintptr 该数据包来自哪个socket
 **************************************************************************/
void TCPServer::requestFriendInviteRejected(DataPackage *pack, qintptr id)
{
    DataBaseManager::getManager()->sqlRejectAddFriend(pack->parameters[0], pack->parameters[1]);
    DataAnalyst tr;
    requestFriendInfo(tr.optToPackage("requestFriendInfo", {pack->parameters[1]}), id);
}

/**************************************************************************
 * @author: 曾群鸿
 * @Date: 2021-8-29
 * @Description: 客户端请求删除好友
 * @Parameters: DataPackege* socket发送的数据包  qintptr 该数据包来自哪个socket
 **************************************************************************/
void TCPServer::requestDeleteFriend(DataPackage *pack, qintptr id)
{
    DataAnalyst transfer;
    DataBaseManager::getManager()->sqlDeleteFriend(pack->parameters[0], pack->parameters[1]);
    if (userIsOnline(pack->parameters[0])) //如果删除方还在线，发送一个好友信息包，用于更新主页面
        requestFriendInfo(transfer.optToPackage("requestFriendInfo", {pack->parameters[0]}), id);
}

/**************************************************************************
 * @author: 曾群鸿
 * @Date: 2021-8-28
 * @Description: 客户端请求向其发送好友邀请的信息
 * @Parameters: DataPackege* socket发送的数据包  qintptr 该数据包来自哪个socket
 **************************************************************************/
void TCPServer::requestFriendInviteList(DataPackage *pack, qintptr id)
{
    DataAnalyst transfer;
    std::vector<QString> res = DataBaseManager::getManager()->sqlGetFriendInviteList(pack->parameters[0]);
    sendToClient(transfer.optToPackage("receiveFriendInviteList", res), id);
}


/**************************************************************************
 * @author: 曾群鸿
 * @Date: 2021-8-26
 * @Description: 将一个数据包发送给一个指定id的在线用户
 * @Parameters: DataPackege* 待发送的数据包  int 待接收用户的id
 **************************************************************************/
void TCPServer::sendToUser(DataPackage *pack, int id)
{
    for (int i = 0; i < onlineUser.size(); ++i)
    {
        std::pair<int, TCPSocket*> now = onlineUser.at(i);
        if (now.first == id)
        {
            DataAnalyst transfer;
            QString s = transfer.packageToQString(pack);
            now.second->write(s.toUtf8());
            break;
        }
    }
}


void TCPServer::requestAllUser(DataPackage *pack, qintptr id)
{
    std::vector<QString> res;
    for (int i = 0; i < onlineUser.size(); ++i)
    {
        int userId = onlineUser.at(i).first;
        res.push_back(DataBaseManager::getManager()->sqlGetUserNameFromId(userId));
    }
    DataAnalyst tr;
    sendToClient(tr.optToPackage("receiveAllUser", res), id);
}

void TCPServer::requestGroupMessage(DataPackage *pack, qintptr id)
{
    sendToAllUser(pack);
    DataBaseManager::getManager()->sqlUpdateGroupChatHistory(pack->parameters[0], pack->parameters[1], pack->parameters[2]);
}

void TCPServer::requestChangeProfile(DataPackage *pack, qintptr id)
{
    DataBaseManager::getManager()->sqlUpdateProfile(pack->parameters[0], pack->parameters[1]);
}


/**************************************************************************
 * @author: 曾群鸿
 * @Date: 2021-8-26
 * @Description: 将一个数据包发送给全部在线用户
 * @Parameters: DataPackege* 待发送的数据包
 **************************************************************************/
void TCPServer::sendToAllUser(DataPackage *pack)
{
    for (int i = 0; i < onlineUser.size(); ++i)
    {
        std::pair<int, TCPSocket*> now = onlineUser.at(i);
        DataAnalyst transfer;
        QString s = transfer.packageToQString(pack);
        now.second->write(s.toUtf8());
    }
}


/**************************************************************************
 * @author: 曾群鸿
 * @Date: 2021-8-26
 * @Description: 将一个数据包发送给一个指定id的客户端
 * @Parameters: DataPackege* 待发送的数据包  qintptr 带接收数据包的socket的标识符
 **************************************************************************/
void TCPServer::sendToClient(DataPackage *pack, qintptr id)
{
    for (int i = 0; i < clientLists.size(); ++i)
    {
        if (clientLists.at(i)->socketDescriptor() == id)
        {
            qDebug() << pack->opt << " " << pack->length << " " << pack->parameters.size();
            DataAnalyst transfer;
            QString s = transfer.packageToQString(pack);
            clientLists[i]->write(s.toUtf8());
            break;
        }
    }
}


/**************************************************************************
 * @author: 曾群鸿
 * @Date: 2021-8-26
 * @Description: 将一个数据包发送给全部客户端
 * @Parameters: DataPackege* 待发送的数据包
 **************************************************************************/
void TCPServer::sentToAllClient(DataPackage *pack)
{
    for (int i = 0; i < clientLists.size(); ++i)
    {
        DataAnalyst transfer;
        clientLists[i]->write(transfer.packageToQString(pack).toUtf8());
    }
}


/**************************************************************************
 * @author: 曾群鸿
 * @Date: 2021-8-26
 * @Description: 当一个socket从服务器上断开连接
 * @Parameters: qintptr 断开连接的socket的标识符
 **************************************************************************/
void TCPServer::clientDisconnect(qintptr id)
{

    for (int i = 0; i < onlineUser.size(); ++i)     //如果这个socket是一个登录的用户，那么将该用户注销
    {
        std::pair<int, TCPSocket*> pii = onlineUser.at(i);
        if (pii.second->getDescriptor() == id)
        {
            qDebug() << "user Id :" << pii.first << " socket Id : " << id << " leave onlineUser!";
            onlineUser.removeAt(i);
            break;
        }
    }
    for (int i = 0; i < clientLists.size(); ++i)    //将这个socket从socket列表中删去
    {
        TCPSocket *now = clientLists.at(i);
        if (now->getDescriptor() == id)
        {
            qDebug() << "socket Id: " << id << " leave server!";
            clientLists.removeAt(i);
            break;
        }
    }
    for (int i = 0; i < onlineUser.size(); ++i)
    {
        std::pair<int, TCPSocket*> pii = onlineUser.at(i);
        QString userName = DataBaseManager::getManager()->sqlGetUserNameFromId(pii.first);
        DataAnalyst tr;
        requestFriendInfo(tr.optToPackage("requestFriendInfo", {userName}), descriptorOfUser(userName));
    }
}


/**************************************************************************
 * @author: 曾群鸿
 * @Date: 2021-8-26
 * @Description: 判断一个用户是否在线
 * @Parameters: QString 待判断的用户
 **************************************************************************/
bool TCPServer::userIsOnline(QString userName)
{
    int userId = DataBaseManager::getManager()->sqlGetIdFromUserName(userName);
    for (int i = 0; i < onlineUser.size(); ++i)
    {
        std::pair<int, TCPSocket*> pii = onlineUser.at(i);
        if (pii.first == userId) return true;
    }
    return false;
}


/**************************************************************************
 * @author: 曾群鸿
 * @Date: 2021-8-26
 * @Description: 判断一个用户是否在线
 * @Parameters: int 待判断用户的id
 **************************************************************************/
bool TCPServer::userIsOnline(int userId)
{
    for (int i = 0; i < onlineUser.size(); ++i)
    {
        std::pair<int, TCPSocket*> pii = onlineUser.at(i);
        if (pii.first == userId) return true;
    }
    return false;
}


/**************************************************************************
 * @author: 曾群鸿
 * @Date: 2021-8-26
 * @Description: 将一个用户强制下线
 * @Parameters: QString 待强制下线的用户名
 **************************************************************************/
void TCPServer::userOffLine(QString userName)
{
    int userId = DataBaseManager::getManager()->sqlGetIdFromUserName(userName);
    for (int i = 0; i < onlineUser.size(); ++i)
    {
        std::pair<int, TCPSocket*> pii = onlineUser.at(i);
        if (pii.first == userId)
        {
            onlineUser.removeAt(i);
            return;
        }
    }
}


/**************************************************************************
 * @author: 曾群鸿
 * @Date: 2021-8-26
 * @Description: 将一个用户强制下线
 * @Parameters: int 强制下线的用户id
 **************************************************************************/
void TCPServer::userOffLine(int userId)
{
    for (int i = 0; i < onlineUser.size(); ++i)
    {
        std::pair<int, TCPSocket*> pii = onlineUser.at(i);
        if (pii.first == userId)
        {
            onlineUser.removeAt(i);
            return;
        }
    }
}


/**************************************************************************
 * @author: 曾群鸿
 * @Date: 2021-8-26
 * @Description: 通过用户名获取该用户的socket标识符
 * @Parameters: QString 用户名
 **************************************************************************/
qintptr TCPServer::descriptorOfUser(QString userName)
{
    int userId = DataBaseManager::getManager()->sqlGetIdFromUserName(userName);
    for (int i = 0; i < onlineUser.size(); ++i)
    {
        if (onlineUser.at(i).first == userId)
        {
            return onlineUser.at(i).second->getDescriptor();
        }
    }
    return -1;
}


/**************************************************************************
 * @author: 曾群鸿
 * @Date: 2021-8-26
 * @Description: 通过用户id获取该用户的socket标识符
 * @Parameters: int 用户id
 **************************************************************************/
qintptr TCPServer::descriptorOfUser(int userId)
{
    for (int i = 0; i < onlineUser.size(); ++i)
    {
        if (onlineUser.at(i).first == userId)
        {
            return onlineUser.at(i).second->getDescriptor();
        }
    }
    return -1;
}

