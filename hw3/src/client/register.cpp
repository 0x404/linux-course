#include "register.h"
#include "ui_register.h"
#include "common.h"
#include "dataanalyst.h"
#include <QMessageBox>

Register::Register(QWidget *parent) :
    QDialog (parent, Qt::WindowTitleHint | Qt::CustomizeWindowHint),
    ui(new Ui::Register)
{
    ui->setupUi(this);
    this->setWindowTitle("注册");

    connect(common::getSocket(), SIGNAL(readyRead()), this, SLOT(getData()));
}

Register::~Register()
{
    delete ui;
}

void Register::getData()
{
    DataAnalyst transfer;
    QString input = common::getSocket()->readAll();
    DataPackage *pack = transfer.QStringToPackage(input);

    if (pack->opt == transfer.getOptId("registerAccepted"))
    {
        disconnect(common::getSocket(), SIGNAL(readyRead()), this, SLOT(getData()));
        QMessageBox::information(nullptr, "accpted", "register accepted!");
        disconnect(common::getSocket(), SIGNAL(readyRead()), this, SLOT(getData()));
        accept();
    }

    if (pack->opt == transfer.getOptId("registerError"))
    {
        QMessageBox::warning(nullptr, "error", "userName has been used!");
    }

}


void Register::on_registerBtn_clicked()
{
    QString userName = ui->userNameText->text();
    QString psw1 = ui->pswText->text();
    QString psw2 = ui->confirmPswText->text();
    if (!userName.length() || !psw1.length() || !psw2.length())
    {
        QMessageBox::warning(nullptr, "error", "Not allow blank");
        return;
    }
    if (psw1 != psw2)
    {
        ui->pswText->clear();
        ui->confirmPswText->clear();
        ui->pswText->setFocus();
        QMessageBox::warning(nullptr, "error", "psw1 must equal to psw2");
        return;
    }
    bool hasLowerLetter = false;
    for (int i = 0; i < psw1.length(); ++i)
        if (psw1[i] >= 'a' && psw1[i] <= 'z') hasLowerLetter = true;
    bool hasUpperLetter = false;
    for (int i = 0; i < psw1.length(); ++i)
        if (psw1[i] >= 'A' && psw1[i] <= 'Z') hasUpperLetter = true;
    bool hasDigit = false;
    for (int i = 0; i < psw1.length(); ++i)
        if (psw1[i] >= '0' && psw1[i] <= '9') hasDigit = true;
    if (psw1.length() < 5 || psw1.length() > 10)
    {
        QMessageBox::warning(nullptr, "error", "pass word length shoud between 5 and 10");
        return;
    }
    else if (!hasLowerLetter)
    {
        QMessageBox::warning(nullptr, "error", "pass word should huave lower letter");
        return;
    }
    else if (!hasUpperLetter)
    {
        QMessageBox::warning(nullptr, "error", "pass word should have upper letter");
        return;
    }
    else if (!hasDigit)
    {
        QMessageBox::warning(nullptr, "error", "pass word should have digit");
        return;
    }

    DataAnalyst transfer;
    common::getSocket()->write(transfer.optToQString("register", {userName, psw1}).toUtf8());
}

void Register::on_registerBtn_2_clicked()
{
    disconnect(common::getSocket(), SIGNAL(readyRead()), this, SLOT(getData()));
    accept();
}
