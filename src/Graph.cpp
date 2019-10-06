#include "Graph.h"

//================ Constructors ================

Node::Node(int* label, string* name)
    : next(nullptr)
{
    this->label = label;
    this->name = name;
};

Node::Node(int* label)
{
    Node(label, nullptr);
};

Node::Node()
{
    Node(nullptr, nullptr);
};

Graph::Graph(int size)
    : nodes(size, 0)
    , labels(size, 0)
    , names(size, string(""))
{
    for (int i = 0; i < size; ++i) {
        labels[i] = i;
        nodes[i] = Node(&labels[i], &names[i]);
    }
}

Graph::Graph()
    : Graph(0) {};

// Create a new graph with deep copies of g's assigned nodes
Graph::Graph(const Graph& g)
    : nodes(g.size(), 0)
    , labels(g.size(), 0)
    , names(g.size(), string(""))
{
    copy(g);
}

// Remove the graphs current nodes and assign it deep copies of g's assigned nodes
void Graph::operator=(const Graph& g)
{

    removeNodes();

    nodes.resize(g.size());
    labels.resize(g.size());
    names.resize(g.size());

    copy(g);
}

// Deep copy of the graph g,  allocating new nodes
void Graph::copy(const Graph& g)
{
    for (int i = 0; i < g.size(); ++i) {
        labels[i] = g.getLabel(i);
        nodes[i] = Node(&labels[i], &names[i]);
    }

    for (int i = 0; i < g.size(); ++i)
        if (labels[i] >= 0 && g.getNode(i)->hasNext())
            for (NODE_IT(g.getNode(i)->getNext())) {
                int label = IT->getLabel();
                nodes[i].addNode(&labels[label], &names[label]);
            }
}

//================ Adding nodes ================

// Add a node properly for the adjacency list, which means «to» points to «from» and vice-versa
void Graph::addNode(int to, int from)
{
    Node& t = getNode(to);
    Node& f = getNode(from);
    t.addNode(f);
    f.addNode(t);
}

// Allocate and add a new node n at the back of the adjacency list entry for the current node
void Node::addNode(Node& n)
{
    addNode(n.label, n.getName());
}

// Allocate and add a new node with label and name pointers at the back of the adjacency list entry for the current node
void Node::addNode(int* label, string* name)
{
    if (next)
        next->addNode(label, name);
    else
        next = new Node(label, name);
}

//================ Removing nodes ================

// Remove

void Graph::removeNode(int label)
{
    vector<Node*> remove;
    for (auto& n : nodes)
        if (n == label)
            n.removeNode();
        else
            for (NODE_IT(n))
                if (IT->hasNext() && IT->getNext() == label)
                    remove.push_back(IT);

    for (auto& n : remove)
        n->removeNext();

    labels[label] = -1;
}

// Remove all dynamically allocated nodes in the graph
void Graph::removeNodes()
{
    for (auto& n : nodes)
        n.removeNode();
}

// Remove and unlink the node pointed to by next, but keep its successors in the adjacency list
void Node::removeNext()
{
    if (next) {
        Node* remove = &getNext();
        if (remove->hasNext())
            next = &remove->getNext();
        else
            next = nullptr;
        delete remove;
    }
}

// Remove and unlink all nodes pointed to by next
void Node::removeNode()
{
    if (hasNext()) {
        vector<Node*> neighbors;
        for (NODE_IT(getNext()))
            neighbors.push_back(IT);

        for (auto& n : neighbors)
            delete n;
    }

    next = nullptr;
}

//================ Neighbors ================

// True if the node denoted by label a successor of the current node in the adjacency list
bool Node::hasNeighbor(int label) const
{
    if (*this == label)
        return true;

    if (hasNext())
        return next->hasNeighbor(label);

    return false;
}

// True if the node src is a neighbor of dst, denoted by labels
bool Graph::hasNeighbor(int src, int dst) const
{
    return nodes[src].hasNeighbor(dst);
}

// True if the node src has a neighbor in dst, denoted by labels
bool Graph::hasNeighbor(int src, vector<int> dst) const
{
    for (auto& i : dst)
        if (hasNeighbor(src, i))
            return true;

    return false;
}

// Get a vector of labels for the nodes which are successors of the current node in the adjacency list
vector<int> Node::getNeighbors() const
{
    vector<int> neighbors;
    for (CONST_NODE_IT(getNextConst()))
        neighbors.push_back(IT->getLabel());

    return neighbors;
}

// Get a vector of node pointers for the nodes which are successors of the node denoted by label in the adjacency list
vector<Node*> Graph::getNeighbors(int label)
{
    vector<Node*> neighbors;
    for (NODE_IT(getNode(label).getNext()))
        neighbors.push_back(&nodes[IT->getLabel()]);

    return neighbors;
}

// Get a vector of const node pointers for the nodes which are successors of the node denoted by label in the adjacency list
vector<const Node*> Graph::getNeighborsConst(int label) const
{
    vector<const Node*> neighbors;
    for (CONST_NODE_IT(getNodeConst(label).getNextConst()))
        neighbors.push_back(&nodes[IT->getLabel()]);

    return neighbors;
}

//================ Printing the graph================

// Overload the cout operator to print the graph in the adjacency list format
ostream& operator<<(ostream& s, const Graph& g)
{
    for (auto& n : g.nodes)
        s << n;
    return s;
}

// Overload the cout operator to print the graph in the adjacency list format
ostream& operator<<(ostream& s, const Node& n)
{
    n.print_adj(s);
    return s;
}

// If the node has set a name, print that, otherwise print its label
void Node::print_label(ostream& s, int label) const
{
    if (name && !name->empty())
        s << *name;
    else
        s << label;
}

// Print the current node in the adjacency list format to the ostream
void Node::print_adj(ostream& s) const
{
    if (*label < 0) //node is not active
        return;

    print_label(s, *label);

    if (next) {
        s << " -- ";
        next->print_adj(s);
    } else
        s << '\n';
}

// Print the graph in the dot format. This requires the graph to be surrounded by a "graph <graphname> { … }"-block
void Graph::print_dotfile() const
{
    cout << "graph g {\n";
    for (auto& n : nodes)
        n.print_dot(cout, n.getLabel());
    cout << "}\n";
}

// The dot file format expects each edge to only appear once, while the
// adjacency list has each edge listed to and from the node. To print out
// the graph in the dot format we ignore the next node if it has a smaller
// label than the current. This way each connection from x to y will appear
// only once, since x != y. This is because we assume the graph is simple
// and thus has no loops and no multiple edges.
void Node::print_dot(ostream& s, int root) const
{
    if (root == *label)
        s << "  ";

    if (*label >= root) { // if the current node is smaller, ignore it
        s << " ";
        print_label(s, *label);
    }

    if (root == *label)
        s << " -- {";

    if (next)
        next->print_dot(s, root);
    else
        s << " }\n";
}
