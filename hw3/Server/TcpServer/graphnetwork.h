#ifndef GRAPHNETWORK_H
#define GRAPHNETWORK_H

#include <vector>
#include <algorithm>
#include "tcpsocket.h"
#include "graphnode.h"
#define MAX_CLIENT_COUNT 500

class GraphNetwork
{
public:
    GraphNetwork();
    GraphNetwork(int, std::vector<GraphNode>, std::vector<std::pair<int, int>>);
    void initNetwork(); //初始化网络
private:
    std::vector<GraphNode> client;  //clinet[i] 表示编号为i的客服端，其中i为主键
    std::vector<GraphNode> to[MAX_CLIENT_COUNT];    //to[i]为编号为i的邻接表
    std::vector<std::pair<int, int>> connection;    //连接关系
    int clientCount;

    void add(int, int);
};

#endif // GRAPHNETWORK_H
