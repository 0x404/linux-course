#include "online.h"
#include "ui_online.h"
#include<QIcon>
#include<QToolButton>
//#include"widget.h"
#include<QMessageBox>
online::online(QWidget *parent) :
    QWidget (parent),
    ui(new Ui::online)
{
    ui->setupUi(this);
    QList<QString> nameList;
    nameList<<"好友1"<<"好友2"<<"好友3"<<"好友4"<<"好友5"<<"好友6"<<"好友7"<<"好友8"<<"好友9";
    QStringList iconNameList;
    iconNameList<<"no1"<<"no2"<<"no3"<<"no4"<<"no5"<<"no6"<<"no7"<<"no8"<<"no9";
    QVector< QToolButton *> vector;
    for(int i=0;i<9;i++)
    {
        QToolButton *btn=new QToolButton(this);
        btn->setIcon(QPixmap(QString(":/new/prefix1/%1.png").arg(iconNameList[i])));
        btn->setIconSize(QPixmap(QString(":/new/prefix1/%1.png").arg(iconNameList[i])).size());
        btn->setText(QString("%1").arg(nameList[i]));
        btn->setAutoRaise(true);
        btn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        ui->vlayout->addWidget(btn);
        vector.push_back(btn);
        IsShow.push_back(false);
    }
/*
    for(int i=0;i<9;i++)
    {
        connect(vector[i],&QToolButton::clicked,[=](){
            if(IsShow[i])
            {
                QMessageBox::warning(this,"警告","该聊天框已被打开!");
                return;
            }
            IsShow[i]=true;
            Widget *widget=new Widget(nullptr,vector[i]->text());
            widget->setWindowIcon(vector[i]->icon());
            widget->setWindowTitle(vector[i]->text());
            widget->show();
            //关闭时将对应的IsShow变为false;
            connect(widget,&Widget::closeWidget,this,[=](){
                IsShow[i]=false;
            });
        });
    }
*/
}

online::~online()
{
    delete ui;
}

