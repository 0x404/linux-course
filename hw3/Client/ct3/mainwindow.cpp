#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "common.h"
#include "dataanalyst.h"

#include <qtoolbutton.h>
#include <QDebug>
#include <qlabel.h>
#include <qboxlayout.h>
#include <QProgressBar>
#include <qpushbutton.h>

#include <QAction>
#include <QIcon>
#include <QTime>

const int MainWindow::portraitCount = 26;

const QString MainWindow::BTN_ORIGIN_CSS = QString(
            "background:transparent;border-width:0;border-style:outset;"
            "font: 200 10pt \"微软雅黑\";"
            "margin-left:-75px;"
            "padding-left:100px;"
                );

const QString MainWindow::BTN_PRESSED_CSS =QString(
            "background:transparent;border-width:0;border-style:outset;"
            "color:rgb(18,150,219) ;"
            "font: 200 10pt \"微软雅黑\";"
            "margin-left:-75px;"
            "padding-left:100px;"
            );
const QString MainWindow::BTN_CSS = QString(
            "background:transparent;border-width:0;border-style:outset; "
            "font: 200 10pt \"微软雅黑\";                                 "
            "margin-left:-75px;                                         "
            "padding-left:100px;                                        "
            );


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Easychat");
    userName = common::userName;
    refreshMyInfo();                    //进行用户名以及个性签名的刷新
    connect(common::getSocket(), SIGNAL(readyRead()), this, SLOT(getDataFromServer()));
    DataAnalyst tr;
    common::getSocket()->write(tr.optToQString("requestFriendInfo", {userName}).toUtf8());
}

MainWindow::~MainWindow()
{
    delete ui;
}


//对服务器发来的消息进行处理
void MainWindow::getDataFromServer()
{
    DataAnalyst tr;
    QString input = common::getSocket()->readAll();
    DataPackage *pack = tr.QStringToPackage(input);

    if (pack->opt == tr.getOptId("receiveMyInfo"))
    {
        userName = pack->parameters[0];
        motto = pack->parameters[1];
        profile = pack->parameters[2];
        common::userName = userName;
        common::profile = profile;
        refreshMyInfo();
    }

    if (pack->opt == tr.getOptId("receiveFriendInfo"))
    {
        common::getSocket()->write(tr.optToQString("requestMyInfo", {common::userName}).toUtf8());
        friendList.clear();
        profileList.clear();
        onlineList.clear();
        for (size_t i = 0; i < pack->parameters.size(); i += 3)
        {
            if  (pack->parameters[i + 2] == "1")
            {
                friendList.push_back(pack->parameters[i]);
                profileList.push_back(pack->parameters[i + 1]);
                onlineList.push_back(pack->parameters[i + 2]);
            }
        }
        for (std::size_t i = 0; i < pack->parameters.size(); i += 3)
        {
            if (pack->parameters[i + 2] == "0")
            {
                friendList.push_back(pack->parameters[i]);
                profileList.push_back(pack->parameters[i + 1]);
                onlineList.push_back(pack->parameters[i + 2]);
            }
        }

        showALLFriendList(friendList);
    }

    if (pack->opt == tr.getOptId("receiveFriendInviteList"))
    {

        requestList.clear();
        timeList.clear();
        for (std::size_t i = 0; i < pack->parameters.size(); i += 2)
        {
            requestList.push_back(pack->parameters[i]);
            QString res = "";

            for (int j = 0, cnt = 0; j < pack->parameters[i + 1].size(); ++j)
            {
                if (pack->parameters[i + 1][j] == ':') ++cnt;
                if (cnt == 2) break;
                res += pack->parameters[i + 1][j];
            }
            timeList.push_back(res);
        }



        showALLReuqestMsg(requestList);
    }
}

//好友列表按钮
void MainWindow::on_FriendList_clicked()
{
    ui->FriendList->setStyleSheet(BTN_PRESSED_CSS); //点击好友列表将样式修改为被点击后的样式

    showALLFriendList(friendList);                  //点击好友列表按钮显示好友列表

    ui->requestList->setStyleSheet(BTN_ORIGIN_CSS); //另一个按钮返回初始样式

    ui->requestList->setIcon(QPixmap(QString(":/mainwindowpic/blackaddfriend.png")));   //另一个按钮返回初始样式
    ui->FriendList->setIcon(QPixmap(QString(":/mainwindowpic/bluechat.png")));          //好友列表按钮图标设为蓝色图标

}

//“申请好友” 按钮
void MainWindow::on_requestList_clicked()
{
    ui->requestList->setStyleSheet(BTN_PRESSED_CSS );   //点击后“申请好友按钮变为被点击样式
    DataAnalyst tr;
    common::getSocket()->write(tr.optToQString("requestFriendInviteList", {userName}).toUtf8());

    ui->FriendList->setStyleSheet(BTN_ORIGIN_CSS); //另一个按钮返回初始样式；
    ui->FriendList->setIcon(QPixmap(QString(":/mainwindowpic/blackchat.png")));//另一边回到原来状态

    ui->requestList->setIcon(QPixmap(QString(":/mainwindowpic/blueaddfriend.png")));
}





void MainWindow::showALLReuqestMsg(std::vector<QString> nameList)
{
    curWindow="Request";                //设置当前窗口为好友列表
    ui->listWidget->clear();            //清空列表


    QVector<QToolButton* > acceptVector;//接受好友按键的集合
    QVector<QToolButton* > rejectVector;//拒绝好友按键的集合



    /********添加好友按钮*****************/

    QToolButton* addFrinendBtn = getADDFriendToolBtn();
    QWidget *widget0=new QWidget(this);

    //使用水平布局类
    QHBoxLayout *horLayout0 = new QHBoxLayout();
    //设置左侧、顶部、右侧和底部边距
    horLayout0->setContentsMargins(0, 0, 0, 0);
    //表示各个控件之间的上下间距
    horLayout0->setSpacing(5);
    //添加控件到布局器
    horLayout0->addWidget(addFrinendBtn);
    widget0->setLayout(horLayout0);
    //用代码创建QListWidgetItem控件，与QWidget控件关联，添加到listWidget控件中。
    QListWidgetItem *item0 = new QListWidgetItem();
    QSize size0 = item0->sizeHint();
    item0->setSizeHint(QSize(size0.width(), 60));
    ui->listWidget->addItem(item0);
    widget0->setSizeIncrement(size0.width(), 60);


    //将item与widget联系
    ui->listWidget->setItemWidget(item0, widget0);




    /*--------------制作标题栏----------------------------------------*/
    QWidget *widget=new QWidget(this);

    //使用水平布局类
    QHBoxLayout *horLayout = new QHBoxLayout();
    QLabel *idLabel = new QLabel();                     //序号
    QLabel *nameLabel = new QLabel();                   //用户名
    QLabel *requestTime = new QLabel();                 //申请时间

    QLabel * acceptBtn = new QLabel();        //接受按钮

    //设置不同控件的样式
    idLabel->setFixedSize(40, 30);
    idLabel->setText("序号");
    idLabel->setAlignment(Qt::AlignHCenter);
    idLabel->setStyleSheet("font: 200 10pt \"微软雅黑\";");

    nameLabel->setText("用户名");
    nameLabel->setFixedSize(140,30);
    nameLabel->setAlignment(Qt::AlignHCenter);
    nameLabel->setStyleSheet("font: 200 10pt \"微软雅黑\";");

    requestTime->setText("时间");
    requestTime->setFixedSize(140,30);
    requestTime->setAlignment(Qt::AlignHCenter);
    requestTime->setStyleSheet("font: 200 10pt \"微软雅黑\";");

    acceptBtn->setText("操作");
    acceptBtn->setFixedSize(100, 24);
    acceptBtn->setAlignment(Qt::AlignHCenter);
    acceptBtn->setStyleSheet("font: 200 10pt \"微软雅黑\";");

    //设置左侧、顶部、右侧和底部边距
    horLayout->setContentsMargins(0, 0, 0, 0);
    //表示各个控件之间的上下间距
    horLayout->setSpacing(5);
    //添加控件到布局器
    horLayout->addWidget(idLabel);
    horLayout->addWidget(nameLabel);
    horLayout->addWidget(requestTime);
    horLayout->addWidget(acceptBtn);

    widget->setLayout(horLayout);

    //用代码创建QListWidgetItem控件，与QWidget控件关联，添加到listWidget控件中。
    QListWidgetItem *item = new QListWidgetItem();
    QSize size = item->sizeHint();
    item->setSizeHint(QSize(size.width(), 40));
    ui->listWidget->addItem(item);
    widget->setSizeIncrement(size.width(), 40);


    //将item与widget联系
    ui->listWidget->setItemWidget(item, widget);



    /*-----------------------------------------------*/


    for (int i=0; i < int(nameList.size() ) ; ++i) {
        QWidget *widget=new QWidget(this);

        //使用水平布局类
        QHBoxLayout *horLayout = new QHBoxLayout();
        QLabel *idLabel = new QLabel();                     //申请的序号
        QLabel *nameLabel = new QLabel();                   //申请人的用户名
        QLabel *requestTime = new QLabel();                 //申请时间

        QToolButton * acceptBtn = new QToolButton();        //接受按钮
        QToolButton * rejectBtn = new QToolButton();        //拒绝按钮

        //设置不同控件的样式
        idLabel->setFixedSize(30, 30);
        idLabel->setText(QString::number(i + 1));
        idLabel->setAlignment(Qt::AlignCenter);
        idLabel->setStyleSheet("font: 200 8pt \"微软雅黑\";");

        nameLabel->setText(nameList[i]);
        nameLabel->setFixedSize(140,30);
        nameLabel->setAlignment(Qt::AlignCenter);
        nameLabel->setStyleSheet("font: 200 8pt \"微软雅黑\";");

        requestTime->setText(timeList[i]);
        requestTime->setFixedSize(120,30);
        requestTime->setStyleSheet("font: 200 8pt \"微软雅黑\";");

        acceptBtn->setText("接受");
        acceptBtn->setFixedSize(50, 24);
        acceptBtn->setStyleSheet("border: 1px solid black;                  "
                                 "border-radius: 5px;                       "
                                 "text-decoration: underline;                   "
                                 );

        rejectBtn->setText("拒绝");
        rejectBtn->setFixedSize(50, 24);
        rejectBtn->setStyleSheet("border: 1px solid black;                  "
                                 "border-radius: 5px;                       "
                                 "text-decoration: underline;                   "
                                 );

        //设置左侧、顶部、右侧和底部边距
        horLayout->setContentsMargins(0, 0, 0, 0);
        //表示各个控件之间的上下间距
        horLayout->setSpacing(5);
        //添加控件到布局器
        horLayout->addWidget(idLabel);
        horLayout->addWidget(nameLabel);
        horLayout->addWidget(requestTime);
        horLayout->addWidget(acceptBtn);
        horLayout->addWidget(rejectBtn);

        widget->setLayout(horLayout);

        //5-  用代码创建QListWidgetItem控件，与QWidget控件关联，添加到listWidget控件中。
        QListWidgetItem *item = new QListWidgetItem();
        QSize size = item->sizeHint();
        item->setSizeHint(QSize(size.width(), 50));
        ui->listWidget->addItem(item);
        widget->setSizeIncrement(size.width(), 50);


        //将item与widget联系

        ui->listWidget->setItemWidget(item, widget);

        acceptVector.push_back(acceptBtn);
        rejectVector.push_back(rejectBtn);

    }

    for (int i=0;i< int(nameList.size());++i) {
        connect(acceptVector[i],&QToolButton::clicked,[=](){
            //接受好友
            DataAnalyst tr;
            common::getSocket()->write(tr.optToQString("friendInviteAccepted", {nameList[i], userName}).toUtf8());

        });

        connect(rejectVector[i],&QToolButton::clicked,[=](){
           DataAnalyst tr;
           common::getSocket()->write(tr.optToQString("friendInviteRejected", {nameList[i], userName}).toUtf8());

        });

    }



}

void MainWindow::showALLFriendList(std::vector<QString> nameList)
{

    ui->FriendList->setStyleSheet(BTN_PRESSED_CSS);
    ui->requestList->setStyleSheet(BTN_ORIGIN_CSS); //另一个按钮返回初始样式；
    ui->requestList->setIcon(QPixmap(QString(":/mainwindowpic/blackaddfriend.png")));
    ui->FriendList->setIcon(QPixmap(QString(":/mainwindowpic/bluechat.png")));
    curWindow="FriendList";//设置当前窗口为好友列表

    ui->listWidget->clear();
    qListWidgetItemList.clear();


    /*---添加群聊按钮---------------------------------*/

    QToolButton* groupChatBtn = getGroupChatBtn();
    QWidget *widget0=new QWidget(this);

    //使用水平布局类
    QHBoxLayout *horLayout0 = new QHBoxLayout();
    //设置左侧、顶部、右侧和底部边距
    horLayout0->setContentsMargins(0, 0, 0, 0);
    //表示各个控件之间的上下间距
    horLayout0->setSpacing(5);
    //添加控件到布局器
    horLayout0->addWidget(groupChatBtn);
    widget0->setLayout(horLayout0);
    //用代码创建QListWidgetItem控件，与QWidget控件关联，添加到listWidget控件中。
    QListWidgetItem *item0 = new QListWidgetItem();
    QSize size0 = item0->sizeHint();
    item0->setSizeHint(QSize(size0.width(), 50));
    ui->listWidget->addItem(item0);
    widget0->setSizeIncrement(size0.width(), 50);


    //将item与widget联系
    ui->listWidget->setItemWidget(item0, widget0);


    /*------------列出好友列表---------------------*/
    for (std::size_t i = 0; i < nameList.size(); ++i)
    {
        QWidget *widget=new QWidget(this);

        QHBoxLayout *horLayout = new QHBoxLayout();
        QLabel *nameLabel = new QLabel();                   //好友的用户名
        /*设置不同控件的样式*/
        //头像

        QToolButton *btnPortrait=new QToolButton(this);

        btnPortrait->setIcon(QPixmap(QString(":/portrait/no%1.jpg").arg(profileList[i])));

        QPixmap* sizemould=new QPixmap(QString(":/new/prefix1/sizemould.bmp"));  //设置头像大小
        btnPortrait->setIconSize(sizemould->size());

        btnPortrait->setStyleSheet("background:transparent;");

        nameLabel->setText(nameList[i]);
        if (onlineList[i] == "1")
            nameLabel->setStyleSheet("font: 200 10pt \"微软雅黑\";");
        else
            nameLabel->setStyleSheet("font: 200 10pt \"微软雅黑\"; color : rgb(117,117,117);");
        nameLabel->setFixedSize(150, 24);
        nameLabel->setAlignment(Qt::AlignCenter);


        //设置左侧、顶部、右侧和底部边距
        horLayout->setContentsMargins(0, 2.5, 0, 2.5);
        //表示各个控件之间的上下间距
        horLayout->setSpacing(5);
        //添加控件到布局器
        horLayout->addWidget(btnPortrait);
        horLayout->addWidget(nameLabel);

        widget->setLayout(horLayout);

        //5-  用代码创建QListWidgetItem控件，与QWidget控件关联，添加到listWidget控件中。
        QListWidgetItem *item = new QListWidgetItem();
        QSize size = item->sizeHint();
        item->setSizeHint(QSize(size.width(), 60));
        ui->listWidget->addItem(item);
        widget->setSizeIncrement(size.width(), 60);


        //将item与widget联系

        ui->listWidget->setItemWidget(item, widget);

        qListWidgetItemList.push_back(item);
    }

}



QToolButton * MainWindow::getADDFriendToolBtn()
{
    QToolButton *btn=new QToolButton(this);
    btn->setIcon(QPixmap(QString(":/mainwindowpic/addfriend.png")));
    QPixmap* sizemould=new QPixmap(QString(":/new/prefix1/sizemould.bmp"));//用另一张图片当模板，来确定其他图片的大小
    btn->setIconSize(sizemould->size());

    btn->setText(QString("添加好友"));
    btn->setStyleSheet(BTN_CSS);
    btn->showMenu();
    btn->setAutoRaise(true);
    btn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    connect(btn, &QToolButton::clicked,[=](){
        //接受好友
        add_friend *afd = new add_friend();
        afd->exec();

    });
    return btn;

}

//返回进行群聊的按钮
QToolButton *MainWindow::getGroupChatBtn()
{
    QToolButton *btn=new QToolButton(this);
    btn->setIcon(QPixmap(QString(":/mainwindowpic/enter_groupchat.png")));
    QPixmap* sizemould=new QPixmap(QString(":/new/prefix1/sizemould.bmp"));//用另一张图片当模板，来确定其他图片的大小
    btn->setIconSize(sizemould->size());

    btn->setText(QString("                      进入世界群聊"));
    btn->setStyleSheet(BTN_CSS);
    btn->showMenu();
    btn->setAutoRaise(true);
    btn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    connect(btn, &QToolButton::clicked,[=](){
        //世界群聊
        talkpageMany* tlkGroup = new talkpageMany();
        disconnect(common::getSocket(), SIGNAL(readyRead()), this, SLOT(getDataFromServer()));
        if (tlkGroup->exec() == QDialog::Accepted)
        {
            connect(common::getSocket(), SIGNAL(readyRead()), this, SLOT(getDataFromServer()));
        }


    });
    return btn;
}



//点击头像将换头像
void MainWindow::on_portraitBtn_clicked()
{
    QList < int > list;
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));  //不能放在循环里，否则容易出现很多相同随机数

    int randomNum = qrand()%(25);

    DataAnalyst tr;
    common::getSocket()->write(tr.optToQString("requestChangeProfile", {common::userName, QString::number(randomNum)}).toUtf8());

    QPixmap portrait =    QPixmap(QString(":/portrait/no%1.jpg").arg(randomNum));

    ui->portraitBtn->setIcon(portrait);
    common::profile = QString::number(randomNum);

}


//监听是否被右键
void MainWindow::on_listWidget_itemPressed(QListWidgetItem *item)
{

    if (!( (QGuiApplication::mouseButtons() & Qt::RightButton) && curWindow=="FriendList")) return;

    qDebug() << "右键";

    QMenu* menu = new QMenu();
    QAction * delAction=new QAction;
    delAction->setText("    删除好友        ");

    int num=0;
    for (; num< qListWidgetItemList.size();num++) {
        if(qListWidgetItemList[num]==item){
            break;
        }
    }

    qDebug() << "第几个框" << num ;

    if(num >= qListWidgetItemList.size()) return; //点击世界群聊按钮时，num会大于集合的大小

    delAction->setObjectName(QString("%1").arg( friendList[num] ));

    //设置动作名称为对应的要删除的用户名，以传递到槽函数之中
    connect(delAction,SIGNAL(triggered(bool)),this,SLOT(on_btnDel_clicked()));
    menu->addAction(delAction);
    menu->exec(QCursor::pos());


}

void MainWindow::on_btnDel_clicked()
{
   qDebug() << "删除好友";
//   QWidget* w =   qobject_cast<QAction*>(sender());

   QString name = sender()->objectName();
   DataAnalyst tr;
   common::getSocket()->write(tr.optToQString("deleteFriend", {userName, name}).toUtf8());

}


//弹出对话框
void MainWindow::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{

    for (int i = 0; i < qListWidgetItemList.size(); ++i)
    {
        if (qListWidgetItemList[i] == item)
        {
            disconnect(common::getSocket(), SIGNAL(readyRead()), this, SLOT(getDataFromServer()));
            talkpage *tkp = new talkpage(nullptr, friendList[i], profileList[i]);
            tkp->setWindowTitle(friendList[i]);
            if (tkp->exec() == QDialog::Accepted)
            {
                connect(common::getSocket(), SIGNAL(readyRead()), this, SLOT(getDataFromServer()));

            }
        }
    }
}


void MainWindow::refreshMyInfo()
{
    ui->userNameLabel->setText(userName);
    ui->mottoEdit->setText(motto);
    QPixmap portrait = QPixmap(QString(":/portrait/no%1.jpg").arg(profile));
    ui->portraitBtn->setIcon(portrait);

}

//修改个性签名
void MainWindow::on_mottoEdit_editingFinished()
{
    QString text = ui->mottoEdit->text();
    DataAnalyst tr;
    common::getSocket()->write(tr.optToQString("requestUpdateMotto", {common::userName, text}).toUtf8());
}


//单击进入聊天界面
void MainWindow::on_listWidget_itemClicked(QListWidgetItem *item)
{
    qDebug() << "单击";
    for (int i = 0; i < qListWidgetItemList.size(); ++i)
    {
        if (qListWidgetItemList[i] == item)
        {
            disconnect(common::getSocket(), SIGNAL(readyRead()), this, SLOT(getDataFromServer()));
            talkpage *tkp = new talkpage(nullptr, friendList[i], profileList[i]);
            tkp->setWindowTitle(friendList[i]);
            if (tkp->exec() == QDialog::Accepted)
            {
                connect(common::getSocket(), SIGNAL(readyRead()), this, SLOT(getDataFromServer()));

            }
        }
    }
}
