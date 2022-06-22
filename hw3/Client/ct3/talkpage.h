#ifndef TALKPAGE_H
#define TALKPAGE_H

#include <QWidget>
#include<QListWidgetItem>
#include"talkmessage.h"
#include <QDialog>


namespace Ui {
class talkpage;
}

class talkpage : public QDialog
{
    Q_OBJECT

public:
    explicit talkpage(QWidget *parent,QString, QString);
    ~talkpage();

    void showTime(QString time);
    void sendMessage(talkmessage *messageW, QListWidgetItem *item, QString text, QString time, talkmessage::msg_type type);//发送对话使用该函数
    void setOtherIcon(QString icon) { icon_other=icon; }
    void setSelfIcon(QString icon) { icon_self=icon; }
    void setSelfid(QString id) { id_self=id; }
    void setOtherid(QString id) { id_other=id; }
    QString userName;
    QString profile;
private slots:
    void on_pushButton_1_clicked();
    void getDataFromServer();
    void on_pushButton_3_clicked();

private:
    QString icon_other;
    QString icon_self;
    QString id_self;
    QString id_other;
    Ui::talkpage *ui;
};

#endif // TALKPAGE_H
