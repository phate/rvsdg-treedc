#include "Kleinberg.h"

// Get a random start bag, by using the heuristic of choosing node 0 and its edges
Graph Graph::start() const
{
    vector<int> labels;
    // TODO: hacked to generate example graph

    // for (CONST_NODE_IT(nodes[0]))
    //     labels.push_back(IT->getLabel());

    labels.push_back(2);
    labels.push_back(4);
    labels.push_back(5);

    return subgraph(labels);
}

// the vector of labels is enpty if it is empty or contain only one label of value -1
bool empty(vector<int>& C)
{
    return C.size() <= 1 && C[0] == -1;
}

// First component found in the graph
Graph Graph::get_component() const
{
    vector<int> C = DFS(size() - 1);
    int i = size() - 1;
    while (i >= 0 && empty(C))
        C = DFS(i--);

    if (empty(C))
        cout << "error: cannot find component" << endl;

    return subgraph(C);
}

// Set of neighbors of U: nodes in u that have a neighbor in c for u in U that have an edge to c in C
Graph Graph::get_neighbors(const Graph& C, const Graph& U) const
{
    vector<int> cnodes = C.activeNodes();
    vector<int> unodes = U.activeNodes();
    vector<int> X;

    for (int u : unodes)
        if (hasNeighbor(u, cnodes))
            X.push_back(u);

    if (X.size() >= 3) // TODO: implement the |X| >= 3 case
        cout << "NB: |X| >= 3";

    return subgraph(X);
}

// new bag V_s = X ∪ {v}
// tries to set v as a neighbor of X, if not picks the first node in C
Graph Graph::new_bag(const Graph& C, const Graph& X) const
{
    vector<int> cnodes = C.activeNodes();
    vector<int> xnodes = X.activeNodes();
    int v = -1;

    for (int c : cnodes)
        if (hasNeighbor(c, xnodes)) {
            v = c;
            break;
        }

    if (v < 0)
        v = cnodes[0];

    Graph g = Graph(unionElement(xnodes, v));
    return g;
}

// Creates a single tree representation from a sets of bags(pieces).
Graph tree_from_bags(vector<Graph>& bags)
{
    Graph Tree = Graph(bags.size());

    // nodes in the tree gets a name corresponding to the nodes in the bag
    for (int i = 0; i < Tree.size(); ++i) {
        stringstream ssname;
        vector<int> nodes = bags[i].activeNodes();
        for (int n : nodes)
            ssname << n;

        // get a char representation of the label (0->a, 1->b, ...)
        string name = ssname.str();
        for (char& n : name)
            n += '1';

        Tree.setName(i, name);
    }

    // to figure out what nodes in the tree to connect, use the heuristic of of connecting the bags
    // with most edges between them in G
    for (size_t i = 0; i < bags.size(); ++i) {
        int max_common = 0;
        pair<int, int> max_common_elem;
        for (size_t j = 0; j < bags.size(); j++) {
            if (i == j || Tree.getNode(i).hasNext()) // only add one neighbor per bag
                continue;

            int new_max_common = bags[i].commonNodes(bags[j]);
            if (new_max_common > max_common) {
                max_common = new_max_common;
                max_common_elem = make_pair(i, j);
            }
        }

        if (max_common > 0)
            Tree.addNode(max_common_elem.first, max_common_elem.second);
    }

    return Tree;
}

// Finds a tree-decomposition from the graph G based on Kleinberg & Tardos
Graph Kleinberg(Graph G, bool debug)
{
    vector<Graph> bags;

    Graph U = G.start();
    bags.push_back(Graph(U));

    if (debug)
        cout << "U\n"
             << U << endl;

    while (!U.completeSubgraph()) {
        Graph GminU = G.minus(U);
        if (debug)
            cout << "G-U\n"
                 << GminU << endl;

        Graph C = GminU.get_component();
        if (debug)
            cout << "C\n"
                 << C << endl;

        Graph X = G.get_neighbors(C, U);
        if (debug)
            cout << "X\n"
                 << X << endl;

        Graph Vs = Graph(G.new_bag(C, X));
        if (debug)
            cout << "Vs\n"
                 << Vs << endl;

        bags.push_back(Vs);

        U = G.unionGraph(U, Vs);

        if (debug)
            cout << "nU\n"
                 << U << endl;
    }

    if (debug) {
        cout << "Bags\n";
        for (auto& b : bags)
            cout << b << endl;
    }

    Graph Tree = tree_from_bags(bags);
    if (debug)
        cout << Tree;

    return Tree;
}
