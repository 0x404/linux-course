#include "login.h"
#include "ui_login.h"
#include "dataanalyst.h"
#include "common.h"
#include <QMessageBox>

Login::Login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);
    this->setWindowTitle("登录");

    common::getSocket()->connectToHost("127.0.0.1", 5500);
    connect(common::getSocket(), SIGNAL(readyRead()), this, SLOT(getData()));
}

Login::~Login()
{
    delete ui;
}

void Login::getData()
{
    DataAnalyst transfer;
    QString input = common::getSocket()->readAll();
    DataPackage *pack = transfer.QStringToPackage(input);
    if (pack->opt == transfer.getOptId("loginAccepted"))
    {
        disconnect(common::getSocket(), SIGNAL(readyRead()), this, SLOT(getData()));

        accept();
    }
    else if (pack->opt == transfer.getOptId("loginError"))
    {
        qDebug() << "login error";
        QMessageBox::warning(nullptr, "loginError", "userName or PassWord invalid!");
    }
}

void Login::on_loginBtn_clicked()
{

    QString userName = ui->userNameEdit->text();
    QString passwd = ui->passWdEdit->text();
    DataAnalyst transfer;
    common::userName = userName;
    common::getSocket()->write(transfer.optToQString("login", {userName, passwd}).toUtf8());
}

void Login::on_registerBtn_clicked()
{
    Register *reg = new Register();
    disconnect(common::getSocket(), SIGNAL(readyRead()), this, SLOT(getData()));
    if (reg->exec() == QDialog::Accepted)
    {
        connect(common::getSocket(), SIGNAL(readyRead()), this, SLOT(getData()));
    }
}
