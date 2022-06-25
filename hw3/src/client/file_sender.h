#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QFile>
#include <QDialog>
#include <QNetworkInterface>
#include <QUdpSocket>
#include <QTcpSocket>
#include <QTcpServer>
#include <QTextStream>
#include <QDataStream>
#include <QTime>
#include <QFont>
#include <QScrollBar>
#include <QString>
#include <QStringList>
#include <QDebug>
#include <QKeyEvent>
#include <QFileDialog>
#include <QCloseEvent>
#include <QHostAddress>
#include <QHostInfo>
#include <QMessageBox>
#include <QDateTime>
#include "dataanalyst.h"
#include "common.h"

namespace Ui {
class TcpServer;
}

class file_sender : public QDialog
{
    Q_OBJECT

public:
    explicit file_sender(QWidget *parent = 0, QString="");
    ~file_sender();
    void initServer();
    void refused();

protected:
    void closeEvent(QCloseEvent *);

private slots:

    void sendFile();
    void updateClientProgress(qint64 );
    void on_openBth_clicked();

    void on_sendBth_clicked();

private:
    Ui::TcpServer *ui;

    QTcpServer *tcpServer;
    QTcpSocket *clientConnection;
    qint16 tcpPort;
    QFile *localFile ;
    qint64 payloadSize ;
    qint64 TotalBytes ;
    qint64 bytesWritten ;
    qint64 bytestoWrite;
    QString theFileName;
    QString fileName;
    QTime time;
    QByteArray outBlock;
    QString talkto;
};

#endif // TCPSERVER_H
