#include "add_friend.h"
#include "ui_add_friend.h"
#include "common.h"
#include "dataanalyst.h"
add_friend::add_friend(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::add_friend)
{
    ui->setupUi(this);
}

add_friend::~add_friend()
{
    delete ui;
}

void add_friend::on_requestList_clicked()
{
    // 添加好友，获取用户名后，直接向服务器发送请求
    // 如果好友存在，服务器在数据库中添加并发送给对应客户端
    // 如果好友不存在，服务器直接拒绝
    DataAnalyst tr;
    QString targetName = ui->lineEdit->text();
    common::getSocket()->write(tr.optToQString("requestAddFriend", {common::userName, targetName}).toUtf8());
    accept();
}
