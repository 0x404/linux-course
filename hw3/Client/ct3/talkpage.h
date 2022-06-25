#ifndef TALKPAGE_H
#define TALKPAGE_H

#include <QWidget>
#include <QListWidgetItem>
#include "talkmessage.h"
#include "file_sender.h"
#include "file_receiver.h"
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
    void setOtherIcon(QString icon) { icon_other=icon; }    // 设置对话方的头像
    void setSelfIcon(QString icon) { icon_self=icon; }      // 设置自己的头像
    void setSelfid(QString id) { id_self=id; }              // 设置自己的IO
    void setOtherid(QString id) { id_other=id; }            // 设置对话方的ID
    QString userName;
    QString profile;
private slots:
    void getDataFromServer();           // 从服务器获取数据报时的事件解析函数
    void on_pushButton_1_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_4_clicked();

private:
    QString icon_other;
    QString icon_self;
    QString id_self;
    QString id_other;
    Ui::talkpage *ui;
    file_sender *sender;
};

#endif // TALKPAGE_H
