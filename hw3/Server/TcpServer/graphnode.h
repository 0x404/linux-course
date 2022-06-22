#ifndef GRAPHNODE_H
#define GRAPHNODE_H


#include <QObject>
#include "tcpsocket.h"
class GraphNode //图网络节点类
{
public:
    GraphNode();
    GraphNode(int, QString, TCPSocket* _socket = nullptr);
    bool operator < (const GraphNode &);    //定义小于运算符，方便在图网络的邻接表中二分查找想要的socket
private:
    int userId;         //用户ID
    QString userName;   //用户名
    TCPSocket *socket;  //用户所使用的socket
    // other information to be added...
};

#endif // GRAPHNODE_H
