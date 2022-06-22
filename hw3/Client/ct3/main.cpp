#include <QApplication>
#include "login.h"
#include "common.h"
#include "mainwindow.h"

QString userName;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Login lg;
    common::getSocket();    //创建客户端唯一的socket
    if (lg.exec() == QDialog::Accepted){
        MainWindow w;
        w.show();
        return a.exec();
    }
    return 0;
}
