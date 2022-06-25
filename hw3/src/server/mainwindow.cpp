#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDateTime>
#include <QDebug>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    server = new TCPServer();
    connect(server, SIGNAL(getRegister()), this, SLOT(refreshList()));
}

void MainWindow::refreshList()
{
    qDebug() << "!!!!!!!!!";
}

//void MainWindow::on_pushButton_2_clicked()
//{
//    DataBaseManager *manager = DataBaseManager::getManager();
//    QDateTime now = QDateTime::currentDateTime();
//    manager->testInsert("aclie", "bob", "hello? test", now.toString("yyyy.MM.dd hh:mm:ss.zzz ddd"));
//}
