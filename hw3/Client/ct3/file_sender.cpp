#include "file_sender.h"
#include "ui_file_sender.h"
#include <QDebug>
file_sender::file_sender(QWidget *parent, QString talkTo) :
    QDialog(parent),
    ui(new Ui::TcpServer),
    localFile(nullptr),
    tcpServer(nullptr),
    clientConnection(nullptr),
    talkto(talkTo),
    tcpPort(8888)
{
    ui->setupUi(this);
    tcpServer = new QTcpServer(this);
    connect(tcpServer,SIGNAL(newConnection()),this,SLOT(sendFile()));
    initServer();
}


file_sender::~file_sender()
{
    delete ui;
}

void file_sender::initServer()
{
    payloadSize = 64 * 1024;
    TotalBytes = 0;
    bytestoWrite = 0;
    bytesWritten = 0;
    ui->serverStatusLabel->setText(tr("请选择文件"));
    ui->progressBar->reset();
    ui->openBth->setEnabled(true);
    ui->sendBth->setEnabled(false);
    tcpServer->close();
}


void file_sender::sendFile()
{
    ui->sendBth->setEnabled(false);
    clientConnection = tcpServer->nextPendingConnection();
    connect(clientConnection,SIGNAL(bytesWritten(qint64)),this,SLOT(updateClientProgress(qint64)));
    ui->serverStatusLabel->setText(tr("开始传送文件:\n %1!").arg(theFileName));
    qDebug() << clientConnection->peerAddress().toString();
    qDebug() << clientConnection->peerPort();
    localFile = new QFile(fileName);
    if(! localFile->open(QFile::ReadOnly))
    {
        QMessageBox::warning(this,tr("警告"),tr("无法读取文件 %1:\n%2").arg(fileName).arg(localFile->errorString()));
        return ;
    }
    TotalBytes = localFile->size();

    QDataStream sendOut(&outBlock,QIODevice::WriteOnly);
    sendOut.setVersion(QDataStream::Qt_4_0);
    time.start();

    QString currentFile = fileName.right(fileName.size() - fileName.lastIndexOf('/')-1);
    sendOut << qint64(0) <<qint64(0) <<currentFile;

    TotalBytes +=outBlock.size();
    sendOut.device()->seek(0);

    sendOut << TotalBytes << qint64((outBlock.size() - sizeof(qint64)*2));

    bytestoWrite = TotalBytes - clientConnection->write(outBlock);
    outBlock.resize(0);
}

void file_sender::updateClientProgress(qint64 numBytes)
{
    qApp->processEvents();
    bytesWritten +=(int) numBytes;
    if(bytestoWrite > 0)
    {
        outBlock = localFile->read(qMin(bytestoWrite,payloadSize));
        bytestoWrite -=(int)clientConnection->write(outBlock);
        outBlock.resize(0);
    }
    else
    {
        localFile->close();
    }

    ui->progressBar->setMaximum(TotalBytes);
    ui->progressBar->setValue(bytesWritten);

    float useTime = time.elapsed();
    double speed = bytesWritten / useTime;
    ui->serverStatusLabel->setText(tr("已发送 %1MB( %2MB/s)"
                                      "\n共%3MB 已用时:%4秒\n估计剩余时间:%5秒")
                                   .arg(bytesWritten / (1024*1024))
                                   .arg(speed * 1000 /(1024*1024),0,'f',2)
                                   .arg(TotalBytes /(1024*1024))
                                   .arg(useTime/1000,0,'f',0)
                                   .arg(TotalBytes/speed/1000 - useTime/1000,0,'f',0));
    if(bytesWritten == TotalBytes)
    {
        localFile->close();
        tcpServer->close();
        ui->serverStatusLabel->setText(tr("传送文件: %1成功").arg(theFileName));
    }
}



void file_sender::refused()
{
    tcpServer->close();
    ui->serverStatusLabel->setText(tr("对方拒绝接受文件"));
}

void file_sender::closeEvent(QCloseEvent *)
{

}

void file_sender::on_openBth_clicked()
{
    fileName = QFileDialog::getOpenFileName(this);
    if(!fileName.isEmpty())
    {
        theFileName = fileName.right(fileName.size()-fileName.lastIndexOf('/')-1);
        ui->serverStatusLabel->setText(tr("发送文件:%1").arg(theFileName));
        ui->sendBth->setEnabled(true);
        ui->openBth->setEnabled(false);
    }
}

void file_sender::on_sendBth_clicked()
{
    if(!tcpServer->listen(QHostAddress::Any, 8888))
    {
        close();
        return ;
    }
    ui->sendBth->setEnabled(false);
    ui->serverStatusLabel->setText(tr("等待对方的接受......"));
    DataAnalyst tr;
    theFileName = fileName.right(fileName.size()-fileName.lastIndexOf('/')-1);
    common::getSocket()->write(tr.optToQString("sendFile", {common::userName, talkto, "127.0.0.1", "127.0.0.1", theFileName}).toUtf8());
}
