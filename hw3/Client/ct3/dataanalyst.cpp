#include "dataanalyst.h"
#include <algorithm>
#include <QDebug>
DataAnalyst::DataAnalyst()
{
    // ("message", {senderName, targetName, content, sendingDate})  参数长度为4
    // ("login", {userName, passwd})    参数长度为2
    // ("register", {userName, passwd}) 参数长度为2
    // ("requestFriendInfo", {userName})    参数长度为1
    // ("requestMyInfo", {userName})    参数长度为1
    // ("receiveFriendInfo", {friendName1, state1, friendName2, state2, ...}) 好友个数n，参数长度为2n
    // ("receiveMyInfo", {userName, state}) 参数长度为2
    // ("requestAddFriend", {userName1, userName2}) 参数长度为2
    // ("accept", {})   参数长度为0
    // ("error", {})    参数长度为0
    cnt = 0;
    optSet = std::vector<QString>{ "message", "messageAccepted", "messageError",
                                   "login", "loginAccepted", "loginError",
                                   "register", "registerAccepted", "registerError",
                                   "requestMyInfo", "receiveMyInfo", "requestMyInfoError",
                                   "requestFriendInfo", "receiveFriendInfo", "requestFriendInfoError",
                                   "requestAddFriend", "requestAddFriendAccepted", "requestAddFriendError",
                                   "receiveFriendInvite", "friendInviteAccepted", "friendInviteRejected",
                                   "requestFriendInviteList", "receiveFriendInviteList",
                                   "requestChatHistory", "receiveChatHistory",
                                   "deleteFriend", "requestUpdateMotto",
                                   "formatError", "groupMessage", "requestAllUser",
                                   "receiveAllUser", "requestChangeProfile",
                                   "requestGroupChatHistory", "receiveGroupChatHistory" };
    for (auto x : optSet)
        optMp[x] = cnt++;
}

/**************************************************************************
 * @author: 曾群鸿
 * @Date: 2021-8-25
 * @Description: 将一个操作转化成QString的格式，用于socket通信
 * @Parameters: QString 操作名     cmd命令参数
 **************************************************************************/
QString DataAnalyst::optToQString(QString opt, std::vector<QString> cmd)
{
    if (!optExist(opt)) //非法操作
    {
        qDebug() << "操作不存在！";
        return optToQString("formatError", {});
    }

    QString res = digitToQString(optMp[opt]) + "$";
    res += digitToQString(int(cmd.size()));
    if (cmd.size()) res += '$';
    for (int i = 0; i < int(cmd.size()) - 1; ++i)
        res += cmd[i] + "$";
    if (cmd.size() > 0)
        res += cmd.back();

    return res;
}

/**************************************************************************
 * @author: 曾群鸿
 * @Date: 2021-8-25
 * @Description: 将一个QString转成对应的数据包
 * @Parameters: QString 待转换的QString
 **************************************************************************/
DataPackage* DataAnalyst::QStringToPackage(QString s)
{
    QStringList sList = s.split("$");
    if (sList.length() < 2) return  errorPackage("formatError"); //接受到了一个非法信息

    int opt = QStringToDigit(sList[0]);
    int len = QStringToDigit(sList[1]);
    std::vector<QString> vec;
    for (int i = 2; i < sList.size(); ++i)
        vec.push_back(sList[i]);
    return new DataPackage(opt, len, vec);
}


/**************************************************************************
 * @author: 曾群鸿
 * @Date: 2021-8-25
 * @Description: 将一个数据包转成对应的QString
 * @Parameters: DataPackage* 带转换的数据包
 **************************************************************************/
QString DataAnalyst::packageToQString(DataPackage * pack)
{
    QString res = digitToQString(pack->opt) + "$";
    res += digitToQString(pack->length);
    if (pack->parameters.size()) res += '$';
    for (int i = 0; i + 1 < pack->parameters.size(); ++i)
    {
        res += pack->parameters[i] + "$";
    }
    if (pack->parameters.size() > 0)
        res += pack->parameters.back();
    return res;
}


/**************************************************************************
 * @author: 曾群鸿
 * @Date: 2021-8-25
 * @Description: 将一个操作转成数据包，调用optToQString和QStringToPackage来完成
 * @Parameters: QSring 操作名  vec 操作参数
 **************************************************************************/
DataPackage* DataAnalyst::optToPackage(QString opt, std::vector<QString> vec)
{
    return QStringToPackage(optToQString(opt, vec));
}


/**************************************************************************
 * @author: 曾群鸿
 * @Date: 2021-8-25
 * @Description: 返回一个错误类型的数据包
 * @Parameters: QString 错误类型
 **************************************************************************/
DataPackage* DataAnalyst::errorPackage(QString error)
{
    return new DataPackage(optMp[error], 0, {});
}


/**************************************************************************
 * @author: 曾群鸿
 * @Date: 2021-8-25
 * @Description: 返回一个接受类型的数据包
 * @Parameters: QString 接受类型
 **************************************************************************/
DataPackage* DataAnalyst::acceptPackage(QString acp)
{
    return new DataPackage(optMp[acp], 0, {});
}


/**************************************************************************
 * @author: 曾群鸿
 * @Date: 2021-8-25
 * @Description: 获取操作对应的ID
 * @Parameters: QString 操作名
 **************************************************************************/
int DataAnalyst::getOptId(QString s)
{
   return optMp[s];
}


/**************************************************************************
 * @author: 曾群鸿
 * @Date: 2021-8-25
 * @Description: 实现QString和int类型之间的互相转化
 * @Parameters:
 **************************************************************************/
QString DataAnalyst::digitToQString(int x)
{
    return QString::number(x);
}

int DataAnalyst::QStringToDigit(QString s)
{
    return s.toInt();
}


/**************************************************************************
 * @author: 曾群鸿
 * @Date: 2021-8-25
 * @Description: 判断一个操作是否存在于当前系统支持的操作集中
 * @Parameters: QString 操作名
 **************************************************************************/
bool DataAnalyst::optExist(QString opt)
{
    return optMp.count(opt);
}
