
#ifndef TALKPAGEMANY_H
#define TALKPAGEMANY_H

#include <QDialog>
#include"talkmessagmany.h"
#include<QListWidgetItem>

#include<QStringListModel>


namespace Ui {
class talkpageMany;
}

class talkpageMany : public QDialog
{
    Q_OBJECT

public:
    explicit talkpageMany(QWidget *parent = nullptr,QString talkname="群聊");
    ~talkpageMany();

    void showTime(QString time);
    void sendMessage( TalkMessagMany *messageW, QListWidgetItem *item, QString text, QString time, TalkMessagMany::msg_type type);//发送对话使用改函数
    void setOtherIcon(QString icon) { icon_other=icon; }    // 设置自己的头像
    void setSelfIcon(QString icon) { icon_self=icon; }  // 设置他人的头像
    void setSelfId(QString id){ id_self=id; }           // 设置自己的ID
    void setOtherid(QString id) { id_other=id; }        // 设置他人的ID

    void setOnlineFriends(QList<QString> list);         // 显示当前在线人员，获取在线人员数据报，发送到UI界面上
    void showHistory(std::vector<QString> parameters);  // 显示聊天记录，实质上通过获取记录数据报，然后一条一条发送到UI界面上
private slots:
    void getData();
    void sendData();
    void on_pushButton_1_clicked();
    void on_pushButton_3_clicked();

private:
    QStringList *ql;
    QStringListModel *qlm;

    QString icon_other;
    QString icon_self;
    QString id_self;
    QString id_other;
    Ui::talkpageMany *ui;
};

#endif // TALKPAGEMANY_H

