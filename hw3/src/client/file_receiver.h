#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include<QFile>
#include<QDialog>
#include<QNetworkInterface>
#include<QUdpSocket>
#include<QTcpSocket>
#include<QTcpServer>
#include<QTextStream>
#include<QDataStream>
#include<QTime>
#include<QFont>
#include<QScrollBar>
#include<QString>
#include<QStringList>
#include<QDebug>
#include<QKeyEvent>
#include<QFileDialog>
#include<QCloseEvent>
#include<QHostAddress>
#include<QHostInfo>
#include<QMessageBox>
#include<QDateTime>

namespace Ui {
class TcpClient;
}

class file_receiver : public QDialog
{
    Q_OBJECT

public:
    explicit file_receiver(QWidget *parent = 0);
    ~file_receiver();
    void setFileName(QString fileName);         // 设置待接收文件名
    void setHostAddress(QHostAddress address);  // 设置发送方的IP地址
    void closeEvent(QCloseEvent *);
    QFile *localFile;

private slots:
    void on_tcpClientCancleBtn_clicked();       // 当用户点击取消按钮
    void on_tcpClientCloseBtn_clicked();        // 当用户点击关闭按钮
    void readMessage();                         // 当监听到有数据发送到本客户端，处理接收信息
    void connectToHost();                       // 连接到发送端服务器
    void displayError(QAbstractSocket::SocketError);

private:
    Ui::TcpClient *ui;
    QTcpSocket *tcpClient;
    qint16  tcpPort ;
    QHostAddress hostAddress;
    qint64 TotalBytes ;
    qint64 bytesReceived;
    qint64 fileNameSize ;
    qint64 blockSize;
    QString fileName;
    QTime time;
    QByteArray inBlock;
};

#endif // TCPCLIENT_H
