#include "graphnetwork.h"

GraphNetwork::GraphNetwork()
{

}

GraphNetwork::GraphNetwork(int _clientCount, std::vector<GraphNode> _client, std::vector<std::pair<int, int>> _connection)
    : client(_client), connection(_connection), clientCount(_clientCount)
{
    initNetwork();
}

void GraphNetwork::initNetwork()
{
    for (int i = 0; i < MAX_CLIENT_COUNT; ++i) to[i].clear();
    for (auto &pii : connection)
    {
        add(pii.first, pii.second);
        add(pii.second, pii.first);
    }
    for (int i = 0; i < MAX_CLIENT_COUNT; ++i)
        sort(to[i].begin(), to[i].end());
}

void GraphNetwork::add(int u, int v)
{
    to[u].push_back(client[v]);
}
