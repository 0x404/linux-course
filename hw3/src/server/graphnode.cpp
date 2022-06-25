#include "graphnode.h"

GraphNode::GraphNode()
{

}

GraphNode::GraphNode(int _userId, QString _userName, TCPSocket* _socket)
    : userId(_userId), userName(_userName), socket(_socket)
{

}

bool GraphNode::operator<(const GraphNode & rhs)
{
    return this->userId < rhs.userId;
}
