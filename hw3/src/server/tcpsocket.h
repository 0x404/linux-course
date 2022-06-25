#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#include <QTcpSocket>
class TCPSocket : public QTcpSocket
{
    Q_OBJECT
public:
    TCPSocket(QObject *parent = nullptr);
    qintptr getDescriptor();
    void init(qintptr);
signals:
    void messageComing(QString, qintptr);  //当前socket接受到消息，向服务器发送一个信号
    void clientDisconnect(qintptr);             //当前socket离开服务器，向服务器发送一个信号
protected slots:
    void receiveData();
    void disconnectFromHost();
private:
    qintptr descriptor;
};

#endif // TCPSOCKET_H
