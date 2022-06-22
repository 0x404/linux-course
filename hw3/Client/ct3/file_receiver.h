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
    void setFileName(QString fileName);
    void setHostAddress(QHostAddress address);
    void closeEvent(QCloseEvent *);
    QFile *localFile;

private slots:
    void on_tcpClientCancleBtn_clicked();
    void on_tcpClientCloseBtn_clicked();
    void readMessage();
    void displayError(QAbstractSocket::SocketError);
    void newConnect();

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
