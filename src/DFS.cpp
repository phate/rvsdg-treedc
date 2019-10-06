#include "DFS.h"

// True if n is not in explored
bool not_explored(int n, set<int>& explored)
{
    return explored.count(n) == 0;
}

// Add the node n to the set of explored nodes
void explore(const Node& n, set<int>& explored)
{
    explored.insert(n.getLabel());
}

// Performs a DFS based on a start node n and a set of explored nodes
void Graph::DFS(const Node& n, set<int>& explored) const
{
    explore(n, explored);

    for (CONST_NODE_IT(n.getNext())) {
        int label = IT->getLabel();

        if (not_explored(label, explored))
            DFS(getNodeConst(label), explored);
    }
}

// A vector of labels containing the order of nodes found in a DFS search starting on the node in the argument
vector<int> Graph::DFS(int label) const
{
    set<int> explored;
    DFS(getNodeConst(label), explored);

    vector<int> search;
    std::copy(explored.begin(), explored.end(), std::back_inserter(search)); //return the set as a vector

    return search;
}
