#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidget>
#include <QMenu>
#include <QMouseEvent>
#include <QLineEdit>
#include <QAction>
#include <QIcon>

#include "talkpage.h"
#include "add_friend.h"
#include "talkpagemany.h"
#include <vector>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();



private slots:

    void getDataFromServer();

    void on_FriendList_clicked();               //好友列表显示按钮

    void on_requestList_clicked();              //好友申请显示按钮

    void on_portraitBtn_clicked();              //头像点击按钮

    void on_listWidget_itemPressed(QListWidgetItem *item);  //检测当前是第几个listWidgetItem被点击

    void on_btnDel_clicked();                   //删除好友键

    void on_listWidget_itemDoubleClicked(QListWidgetItem *item);    //双击跳转到与对应的用户进行聊天

    void on_mottoEdit_editingFinished();        //设置个性签名

    void on_listWidget_itemClicked(QListWidgetItem *item);  //单击弹出好友聊天框

signals:
    void returnToOriginCss();

private:
    Ui::MainWindow *ui;

    std::vector<QString> friendList;
    std::vector<QString> onlineList;
    std::vector<QString> requestList;
    std::vector<QString> timeList;
    std::vector<QString> profileList;
    QString userName;
    QString motto;
    QString profile = "11";

    QString curWindow;

    QVector<QListWidgetItem*> qListWidgetItemList;

    void showALLReuqestMsg(std::vector<QString> nameList);
    void showALLFriendList(std::vector<QString> nameList);
    void refreshMyInfo();

    QToolButton * getADDFriendToolBtn();       //添加toolbtn（用来添加好友）
    QToolButton * getGroupChatBtn();           //获取群聊按钮


    static const QString BTN_ORIGIN_CSS;     //申请好友列表与好友列表 的按键原始样式
    static const QString BTN_PRESSED_CSS;    //按下后样式
    static const QString BTN_CSS;            //其他按钮的样式
    static const int portraitCount;          //本地客户端保存了多少张头像
};

#endif // MAINWINDOW_H
