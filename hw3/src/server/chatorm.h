#ifndef CHATORM_H
#define CHATORM_H
#include <QString>

/*
 * @info: 聊天记录相关
 * */
class ChatORM
{
public:
    ChatORM();
    ChatORM(int senderId, int receiverId, QString content, QString time);
    ChatORM(int senderId, QString content, QString time, QString info);
    int getSenderId() const;
    void setSenderId(int value);

    int getReceiverId() const;
    void setReceiverId(int value);

    QString getContent() const;
    void setContent(const QString &value);

    QString getTime() const;
    void setTime(const QString &value);

    QString getInfo() const;
    void setInfo(const QString &value);

private:
    QString info;
    int senderId;
    int receiverId;
    QString content;
    QString time;
};

#endif // CHATORM_H
