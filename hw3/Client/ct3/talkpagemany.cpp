#include "talkpagemany.h"
#include "ui_talkpagemany.h"
#include "talkmessagmany.h"
#include <QDateTime>
#include <QTcpSocket>
#include "dataanalyst.h"
#include "common.h"
#include <stdlib.h>

talkpageMany::talkpageMany(QWidget *parent,QString str) :
    QDialog (parent, Qt::WindowTitleHint | Qt::CustomizeWindowHint),
    ui(new Ui::talkpageMany)
{
    ui->setupUi(this);
    ui->pushButton_2->setText(str);
    setSelfId(common::userName);
    setSelfIcon(common::profile);
    connect(common::getSocket(), SIGNAL(readyRead()), this, SLOT(getData()));
    DataAnalyst tr;
    common::getSocket()->write(tr.optToQString("requestGroupChatHistory", {}).toUtf8());
}

talkpageMany::~talkpageMany()
{
    delete ui;
}

void talkpageMany::getData()
{
    QString input = common::getSocket()->readAll();
    DataAnalyst tr;
    DataPackage *pack = tr.QStringToPackage(input);

    if (pack->opt == tr.getOptId("receiveAllUser"))
    {
        QList<QString> l;
        for (std::size_t i = 0; i < pack->parameters.size(); ++i)
            l.append(pack->parameters[i]);
        setOnlineFriends(l);
    }

    if (pack->opt == tr.getOptId("groupMessage"))
    {
        if (pack->parameters[0] == common::userName)
        {

            QString msg = pack->parameters[1];
            QString time = QString::number(QDateTime::currentDateTime().toTime_t()); //时间戳
            showTime(time);
            TalkMessagMany* message = new TalkMessagMany(ui->listWidget->parentWidget(),icon_self,icon_other,id_self,id_other);
            QListWidgetItem* item = new QListWidgetItem(ui->listWidget);
            sendMessage(message, item, msg, time, TalkMessagMany::user_self);
        }
        else
        {

            QString msg = pack->parameters[1];
            QString time = QString::number(QDateTime::currentDateTime().toTime_t()); //时间戳
            showTime(time);
            TalkMessagMany* message = new TalkMessagMany(ui->listWidget->parentWidget(),icon_self, pack->parameters[2], id_self, pack->parameters[0]);
            QListWidgetItem* item = new QListWidgetItem(ui->listWidget);
            sendMessage(message, item, msg, time, TalkMessagMany::user_other);
        }
        ui->listWidget->setCurrentRow(ui->listWidget->count()-1);
    }

    if (pack->opt == tr.getOptId("receiveGroupChatHistory"))
    {
        std::vector<std::vector<QString>> history;

        for (int i = 0; i < pack->parameters.size(); i += 4)
        {
            history.push_back(std::vector<QString>{pack->parameters[i], pack->parameters[i + 1], pack->parameters[i + 2], pack->parameters[i + 3]});
        }
        for (int i = 0; i < history.size(); ++i)
        {
            showHistory(history[i]);
        }
        ui->listWidget->setCurrentRow(ui->listWidget->count()-1);
        DataAnalyst tr;
        common::getSocket()->write(tr.optToQString("requestAllUser", {}).toUtf8());
    }
}

void talkpageMany::setOnlineFriends(QList<QString> list)
{
    QString str;
    ql=new QStringList;
    qlm=new QStringListModel;
    for(int i=0;i<list.count();i++){
        str=list[i];
        ql->append(str);
    }
    qlm->setStringList(*ql);
    ui->OnlineView->setModel(qlm);
}


void talkpageMany::showTime(QString time)
{
    bool ShowTime = false;
    if(ui->listWidget->count() > 0) {
        QListWidgetItem* lastItem = ui->listWidget->item(ui->listWidget->count() - 1);
        TalkMessagMany* message = (TalkMessagMany*)ui->listWidget->itemWidget(lastItem);
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
        TalkMessagMany* timeMessage = new TalkMessagMany(ui->listWidget->parentWidget(),icon_other);
        QListWidgetItem* itemTime = new QListWidgetItem(ui->listWidget);
        QSize size = QSize(this->width(), 40);
        timeMessage->resize(size);
        itemTime->setSizeHint(size);
        timeMessage->sendText(time, time, size, TalkMessagMany::user_time);
        ui->listWidget->setItemWidget(itemTime, timeMessage);
    }
}
void talkpageMany::sendMessage(TalkMessagMany *message, QListWidgetItem *item, QString text, QString time, TalkMessagMany::msg_type type)
{
    message->setFixedWidth(this->width());
    QSize size = message->fontRect(text);
    item->setSizeHint(size);
    message->sendText(text, time, size, type);
    ui->listWidget->setItemWidget(item, message);
}

void talkpageMany::on_pushButton_1_clicked()
{
    QString msg = ui->textEdit->toPlainText();
    ui->textEdit->setText("");
    QString time = QString::number(QDateTime::currentDateTime().toTime_t()); //时间戳
    if(msg != "")
    {
        DataAnalyst tr;
        common::getSocket()->write(tr.optToQString("groupMessage", {common::userName, msg, common::profile}).toUtf8());
    }
    ui->listWidget->setCurrentRow(ui->listWidget->count()-1);
//    DataAnalyst tr;
}

void talkpageMany::sendData(){

}

void talkpageMany::on_pushButton_3_clicked()
{
    disconnect(common::getSocket(), SIGNAL(readyRead()), this, SLOT(getData()));
    accept();
}

void talkpageMany::showHistory(std::vector<QString> parameters)
{
    if (parameters[0] == common::userName)
    {
        QString msg = parameters[1];
        QString time = parameters[2];
        QString picId = parameters[3];
        QDateTime t = QDateTime::fromString(time, "yyyy-MM-dd hh:mm:ss");
        QString time_real = QString::number(t.toTime_t());
        showTime(time_real);
        TalkMessagMany* message = new TalkMessagMany(ui->listWidget->parentWidget(),icon_self,icon_other,id_self,id_other);
        QListWidgetItem* item = new QListWidgetItem(ui->listWidget);
        sendMessage(message, item, msg, time, TalkMessagMany::user_self);
    }
    else
    {
        QString msg = parameters[1];
        QString time = parameters[2];
        QString picId = parameters[3];
        QDateTime t = QDateTime::fromString(time, "yyyy-MM-dd hh:mm:ss");
        QString time_real = QString::number(t.toTime_t());
        showTime(time_real);
        TalkMessagMany* message = new TalkMessagMany(ui->listWidget->parentWidget(),icon_self, picId, id_self, parameters[0]);
        QListWidgetItem* item = new QListWidgetItem(ui->listWidget);
        sendMessage(message, item, msg, time, TalkMessagMany::user_other);
    }
}
