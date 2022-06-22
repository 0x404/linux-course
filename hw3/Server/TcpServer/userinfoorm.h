#ifndef USERINFOORM_H
#define USERINFOORM_H
#include <QString>
#include <vector>

/*
 *@info: userinfo数据库表对应的实体类，储存着除了用户密码之外的所有信息
 * */
class UserInfoORM
{
public:
    UserInfoORM();
    UserInfoORM(int id, QString username, QString motto,  std::vector<int> friendList, QString otherinfo="");

    QString toString();                         //将user所有信息转为字符窜输出

    int getId() const;
    void setId(int value);

    QString getUsername() const;
    void setUsername(const QString &value);

    QString getMotto() const;
    void setMotto(const QString &value);

    QString getOtherinfo() const;
    void setOtherinfo(const QString &value);

    std::vector<int> getFriendList() const;
    void setFriendList(const std::vector<int> &value);

private:
    int id;
    QString username;
    QString motto;
    QString otherinfo;
    std::vector<int> friendList;
};

#endif // USERINFOORM_H
