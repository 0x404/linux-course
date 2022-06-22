
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
    void setOtherIcon(QString icon) { icon_other=icon; }
    void setSelfIcon(QString icon) { icon_self=icon; }
    void setSelfId(QString id){ id_self=id; }
    void setOtherid(QString id) { id_other=id; }

    void setOnlineFriends(QList<QString> list);
    void showHistory(std::vector<QString> parameters);
private slots:
    void on_pushButton_1_clicked();
    void getData();

    void sendData();
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

