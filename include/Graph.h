#pragma once
#include "main.h"

class Node;

/*
 * Element in the linked list for the adjacency list representation in graph.
 */

class Node {
private:
    Node* next;
    int* label;
    string* name;

public:
    Node();
    Node(int* label);
    Node(int* label, string* name);

    int getLabel() const { return *label; }
    string* getName() const { return name; }

    bool hasNext() const { return next != nullptr; }
    void setNext(Node* next) { this->next = next; }
    Node& getNext() const { return *next; }
    Node& getNextConst() const { return *next; }

    void addNode(int* label, string* name);
    void addNode(Node& n);

    void removeNext();
    void removeNode();

    bool hasNeighbor(int label) const;
    vector<int> getNeighbors() const;

    void print_label(ostream& s, int label) const;
    void print_adj(ostream& s) const;
    void print_dot(ostream& s, int root) const;
    friend ostream& operator<<(ostream& s, const Node& n);

    bool operator==(const Node& n) const { return getLabel() == n.getLabel(); }
    bool operator!=(const Node& n) const { return !(*this == n); }
    bool operator==(int label) const { return getLabel() == label; }
    bool operator<(const Node& n) const { return getLabel() < n.getLabel(); }

    class iterator;
    class const_iterator;
};

/*
 * Adjacency list implementation of a graph (https://en.wikipedia.org/wiki/Adjacency_list)
 * Each Row in the list is represented by a singly linked list, where each root is in the vector «nodes».
 * Assumed that the graph is undirected and that the graph is simple (no parallel edges)
 *
 * Instead of each Node in this list having its own value/label, the nodes point to a vector containing values.
 * This is because, in the adjacency list representation, each node has several entries, if we have the graph 0 -- 1,
 * the corresponding entry in the representation is: 0 -- 1, and 1 -- 0. Thus if we want to update a property for a node,
 * we have to iterate the entire list, we have to iterate through the entire graph to update the property for all instances of the node.
 *
 * The «names» field is ment to represent nodes in a bag for a tree-decomposition and is implemented in the same way.
 * It is not certain that this is the best way to implement the adjacency list, and can be changed to a simpler concept if it turns out
 * that updating properties for the nodes is not seldom used.
 */

class Graph {
private:
    vector<Node> nodes;
    vector<int> labels;
    vector<string> names;

public:
    Graph(int size);
    Graph();
    Graph(const Graph& g);
    void copy(const Graph& g);
    void operator=(const Graph& g);

    int size() const { return nodes.size(); }
    int getLabel(int label) const { return labels[label]; }

    Node& getNode(int label) { return nodes[label]; }
    Node& getNode(Node& n) { return nodes[n.getLabel()]; }
    const Node& getNodeConst(int label) const { return nodes[label]; }
    const Node& getNodeConst(Node& n) const { return nodes[n.getLabel()]; }
    const Node* getNode(int label) const { return &nodes[label]; }

    void addNode(int to, int from);
    void removeNode(int label);
    void removeNodes();
    ~Graph() { removeNodes(); }

    void setName(int label, string s) { names[label] = s; };
    string getName(int label) const { return names[label]; };

    bool hasNeighbor(int src, vector<int> dst) const;
    bool hasNeighbor(int src, int dst) const;
    vector<Node*> getNeighbors(int label);
    vector<const Node*> getNeighborsConst(int label) const;

    friend ostream& operator<<(ostream& s, const Graph& g);
    void print_dotfile() const;

    // Operations
    vector<int> activeNodes() const;
    bool completeSubgraph() const;
    int commonNodes(const Graph& g) const;
    Graph minus(vector<int> labels) const;
    Graph minus(const Graph& U) const;
    Graph subgraph(vector<int>& labels) const;
    Graph subgraph(const Graph& U) const;
    Graph unionElement(vector<int>& X, int v) const;
    Graph unionGraph(const Graph& g1, const Graph& g2) const;

    // Kleinberg
    Graph start() const;
    Graph get_component() const;
    Graph get_neighbors(const Graph& C, const Graph& U) const;
    Graph new_bag(const Graph& C, const Graph& X) const;

    // DFS
    void DFS(const Node& n, set<int>& explored) const;
    vector<int> DFS(int label) const;

    // Heuristics
    int min_node(vector<int> nodes, int (Graph::*min_f)(int) const) const;
    void simplify(int label);
    void eliminate(int label);
    int getDegree(int label) const;
    pair<vector<int>, int> min_fill_heuristic();
    int minClique() const;
    int missingFromClique(int label) const;
    pair<vector<int>, int> min_width_heuristic();
    int minDegree() const;
    pair<vector<int>, int> max_cardinality_heuristic();
    int maxLabeledNeighbors(vector<int> ordering) const;
    int minor_min_width_heuristic();
};

#define NODE_IT(x)         \
    Node::iterator it(&x); \
    it != it.end();        \
    ++it
#define CONST_NODE_IT(x)         \
    Node::const_iterator it(&x); \
    it != it.end();              \
    ++it
#define IT (*it)

/* A minimal iterator of the node class */
class Node::iterator {
public:
    Node* itr;

public:
    iterator(Node* temp)
        : itr(temp)
    {}

    iterator& operator++()
    {
        itr = &itr->getNext();
        return *this;
    }

    bool operator==(const iterator& other) { return itr == other.itr; }
    bool operator!=(const iterator& other) { return itr != other.itr; }
    Node* operator*() { return itr; }
    iterator end() { return nullptr; }
};

/* A minimal const iterator of the node class */
class Node::const_iterator {
public:
    const Node* itr;

public:
    const_iterator(const Node* temp)
        : itr(temp)
    {}

    const_iterator& operator++()
    {
        itr = &itr->getNext();
        return *this;
    }

    bool operator==(const const_iterator& other) { return itr == other.itr; }
    bool operator!=(const const_iterator& other) { return itr != other.itr; }
    const Node* operator*() const { return itr; }
    const_iterator end() const { return nullptr; }
};

/* implements hash function for Node to allow hash-maps and unordered sets */
// namespace std {
// template <>
// struct hash<Node> {
//     size_t operator()(const Node& n) const
//     {
//         return hash<int>()(n.getLabel());
//     }
// };
// }
