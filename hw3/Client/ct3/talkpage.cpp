#include "talkpage.h"
#include "ui_talkpage.h"
#include "talkmessage.h"
#include "dataanalyst.h"
#include "common.h"
#include <QDebug>
#include <stdlib.h>
#include<QDateTime>

talkpage::talkpage(QWidget *parent,QString str, QString pic) :
    QDialog (parent, Qt::WindowTitleHint | Qt::CustomizeWindowHint),
    ui(new Ui::talkpage)
{
    userName = str;
    profile = pic;
    setWindowIcon(QIcon(":/mainwindowpic/bluechat.png"));
    setSelfIcon(common::profile);
    setSelfid(common::userName);
    setOtherid(userName);
    setOtherIcon(profile);
    ui->setupUi(this);
    ui->pushButton_2->setText(str);
    connect(common::getSocket(), SIGNAL(readyRead()), this, SLOT(getDataFromServer()));
    DataAnalyst tr;
    sender = new file_sender(this, userName);
    common::getSocket()->write(tr.optToQString("requestChatHistory", {common::userName, userName}).toUtf8());
}

talkpage::~talkpage()
{
    qDebug() << "!!";
    delete ui;
}

void talkpage::getDataFromServer()
{
    DataAnalyst translator;
    QString input = common::getSocket()->readAll();
    DataPackage *pack = translator.QStringToPackage(input);
    if (pack->opt == translator.getOptId("message"))
    {
        QString time = QString::number(QDateTime::currentDateTime().toTime_t()); //时间戳
        showTime(time);
        talkmessage* message = new talkmessage(ui->listWidget->parentWidget(), profile, common::profile);
        QListWidgetItem* item = new QListWidgetItem(ui->listWidget);
        sendMessage(message, item, pack->parameters[2], time, talkmessage::user_other);
        ui->listWidget->setCurrentRow(ui->listWidget->count()-1);
    }

    if (pack->opt == translator.getOptId("receiveChatHistory"))
    {
        std::vector<std::vector<QString>> history;
        std::vector<QString> tmp;
        for (std::size_t i = 0; i < pack->parameters.size(); ++i)
        {
            tmp.push_back(pack->parameters[i]);
            if ((i + 1) % 4 == 0)
            {
                history.push_back(tmp);
                tmp.clear();
            }
        }
        for (std::size_t i = 0; i < history.size(); ++i)
        {

            QString sender = history[i][0];
            QString receiver = history[i][1];
            QString content = history[i][2];
            QString time = history[i][3];

            QDateTime t = QDateTime::fromString(time, "yyyy-MM-dd hh:mm:ss");
            QString time_real = QString::number(t.toTime_t());
            showTime(time_real);
            talkmessage* message = new talkmessage(ui->listWidget->parentWidget(), profile, common::profile);
            QListWidgetItem* item = new QListWidgetItem(ui->listWidget);
            sendMessage(message, item, content, time, sender == common::userName ? talkmessage::user_self : talkmessage::user_other);
            ui->listWidget->setCurrentRow(ui->listWidget->count()-1);
        }
    }

    if (pack->opt == translator.getOptId("receiveFile"))
    {
        QString sender = pack->parameters[0];
        QString receiver = pack->parameters[1];
        QString senderIP = pack->parameters[2];
        QString receiverIP = pack->parameters[3];
        QString fileName = pack->parameters[4];
        int btn = QMessageBox::information(this,tr("接收文件"),tr("来自 %1 (%2)的文件:%3","是否接受").arg(sender).arg(senderIP).arg(fileName),QMessageBox::Yes,QMessageBox::No);
        if(btn == QMessageBox::Yes)
        {
            QString name = QFileDialog::getSaveFileName(0,tr("保存文件"),fileName);
            if(!name.isEmpty())
            {
                file_receiver *client = new file_receiver(this);
                client->setFileName(name);
                client->setHostAddress(QHostAddress(senderIP));
                client->show();
            }
        }
    }
}

void talkpage::showTime(QString time)
{
    bool ShowTime = false;
    if(ui->listWidget->count() > 0) {
        QListWidgetItem* lastItem = ui->listWidget->item(ui->listWidget->count() - 1);
        talkmessage* message = (talkmessage*)ui->listWidget->itemWidget(lastItem);
        int last_Time = message->gettime().toInt();
        int now_Time = time.toInt();
        qDebug()<<last_Time;
        qDebug()<<now_Time;
        if((now_Time - last_Time) > 300){
            ShowTime = true;
        }
    }
    else {
        ShowTime = true;
    }
    if(ShowTime==true) {
        talkmessage* timeMessage = new talkmessage(ui->listWidget->parentWidget());
        QListWidgetItem* itemTime = new QListWidgetItem(ui->listWidget);
        QSize size = QSize(this->width(), 40);
        timeMessage->resize(size);
        itemTime->setSizeHint(size);
        timeMessage->sendText(time, time, size, talkmessage::user_time);
        ui->listWidget->setItemWidget(itemTime, timeMessage);
    }
}
void talkpage::sendMessage(talkmessage *message, QListWidgetItem *item, QString text, QString time, talkmessage::msg_type type)
{
    message->setFixedWidth(this->width());
    QSize size = message->fontRect(text);
    item->setSizeHint(size);
    message->sendText(text, time, size, type);
    ui->listWidget->setItemWidget(item, message);

}

void talkpage::on_pushButton_1_clicked()
{
    QString msg = ui->textEdit->toPlainText();
    ui->textEdit->setText("");
    QString time = QString::number(QDateTime::currentDateTime().toTime_t()); //时间戳

//    bool isSending = true; // 发送中
//    if(ui->listWidget->count()%2) {
//        if(isSending) {
//            showTime(time);
//            talkmessage* message = new talkmessage(ui->listWidget->parentWidget());
//            QListWidgetItem* item = new QListWidgetItem(ui->listWidget);
//            sendMessage(message, item, msg, time, talkmessage::user_self);

//        }
//        else {
//            bool isOver = true;
//            for(int i = ui->listWidget->count() - 1; i > 0; i--)
//            {
//                talkmessage* message = (talkmessage*)ui->listWidget->itemWidget(ui->listWidget->item(i));
//                if(message->gettext() == msg) {
//                    isOver = false;

//                }
//            }
//            if(isOver) {
//                showTime(time);
//                talkmessage* message = new talkmessage(ui->listWidget->parentWidget());
//                QListWidgetItem* item = new QListWidgetItem(ui->listWidget);
//                sendMessage(message, item, msg, time, talkmessage::user_self);

//            }
//        }
//    }
        if(msg != "") {
            showTime(time);
            talkmessage* message = new talkmessage(ui->listWidget->parentWidget(), profile, common::profile);
            QListWidgetItem* item = new QListWidgetItem(ui->listWidget);
            sendMessage(message, item, msg, time, talkmessage::user_self);
            DataAnalyst tr;
            common::getSocket()->write(tr.optToQString("message", {common::userName, userName, msg}).toUtf8());
        }
    ui->listWidget->setCurrentRow(ui->listWidget->count()-1);
}

void talkpage::on_pushButton_3_clicked()
{
    disconnect(common::getSocket(), SIGNAL(readyRead()), this, SLOT(getDataFromServer()));
    accept();
    this->close();
}



void talkpage::on_pushButton_4_clicked()
{
    sender->show();
    sender->initServer();
}
