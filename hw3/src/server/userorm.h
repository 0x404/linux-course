#ifndef USERORM_H
#define USERORM_H
#include <QString>

/*
 * @parm: id, username, psw
 * @info: user数据库表对应的表，储存着用户的密码password与用户名username信息
 * */
class UserORM
{
public:
    UserORM();
    UserORM(int id,QString username,QString psw);

    int getQId();
    void setQId(int id);

    QString getUserName();
    void setUserName(QString username);

    QString getPassword();
    void setPassword(QString psw);


private:
    int id;
    QString username;
    QString password;

};

#endif // USERORM_H
