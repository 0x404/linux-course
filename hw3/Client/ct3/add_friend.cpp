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
    DataAnalyst tr;
    QString targetName = ui->lineEdit->text();
    common::getSocket()->write(tr.optToQString("requestAddFriend", {common::userName, targetName}).toUtf8());
    accept();
}
