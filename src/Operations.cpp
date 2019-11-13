#include "Operations.h"

//================ Vector functions ================

// True if element e in vector v
bool in(vector<int>& v, int e)
{
    return find(v.begin(), v.end(), e) != v.end();
}

// Print the vector
void print_vec(vector<int>& vec)
{
    for (int i : vec)
        cout << i << " ";
    cout << endl;
}

//================ Graph operations ================

// A list of labels denoting active nodes in graph
vector<int> Graph::activeNodes() const
{
    vector<int> labels;
    for (int l : this->labels)
        if (l >= 0)
            labels.push_back(l);

    return labels;
}

// True if all nodes are active
bool Graph::completeSubgraph() const
{
    for (int l : labels)
        if (l < 0)
            return false;
    return true;
}

// The number of equal nodes active in the current graph and the subgraph g
int Graph::commonNodes(const Graph& g) const
{
    vector<int> g1nodes = activeNodes();
    vector<int> g2nodes = g.activeNodes();
    int common = 0;
    for (size_t i = 0; i < g1nodes.size(); ++i)
        for (size_t j = 0; j < g2nodes.size(); j++)
            if (g1nodes[i] == g2nodes[j])
                ++common;

    return common;
}

// A copy of the current graph minus a set of nodes denoted by a vector of labels
Graph Graph::minus(vector<int> labels) const
{
    Graph subgraph = Graph(*this);
    for (int i : labels)
        subgraph.removeNode(i);

    return subgraph;
}

// A copy of the current graph minus the set of active nodes in argument subgraph
Graph Graph::minus(const Graph& U) const
{
    return minus(U.activeNodes());
}

// A copy of the current graph which includes the set of nodes denoted by a vector of labels
Graph Graph::subgraph(vector<int>& labels) const
{
    vector<int> r = activeNodes();

    // Remove nodes in r that is also in labels
    for (int l : labels) {
        auto lambda = [l](int a) { return a == l; };
        auto rem = remove_if(r.begin(), r.end(), lambda);
        r.erase(rem, r.end());
    }

    // The remaining nodes in r can be removed to create the subgraph
    return minus(r);
}

// A copy of the current graph which includes the set of active nodes in the argument subgraph
Graph Graph::subgraph(const Graph& U) const
{
    vector<int> active = U.activeNodes();
    return subgraph(active);
}

// A copy of the current graph which is a union of the subgraph X and the element v, denoted by labels
Graph Graph::unionElement(vector<int>& X, int v) const
{
    if (!in(X, v))
        X.push_back(v);

    return subgraph(X);
}

// A copy of the current graph which is a union of the active nodes in the subgraphs g1 and g2
Graph Graph::unionGraph(const Graph& g1, const Graph& g2) const
{
    vector<int> g1nodes = g1.activeNodes();
    vector<int> g2nodes = g2.activeNodes();
    for (int i : g2nodes)
        if (!in(g1nodes, i))
            g1nodes.push_back(i);

    return Graph(subgraph(g1nodes));
}
