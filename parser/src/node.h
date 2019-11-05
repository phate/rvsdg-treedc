#ifndef NODE_H
#define NODE_H

#include "element.h"
#include <string.h>

enum NodeType {
    NODE,
    LAMBDA,
    GAMMA,
    THETA,
    PHI
};

#define NodeTypeLen 5
static const char* NodeTypeStrings[] = { "node", "lambda", "gamma", "theta", "phi" };

NodeType getNodeType(const char str[])
{
    for (int i = 0; i < NodeTypeLen; ++i)
        if (strcmp(NodeTypeStrings[i], str) == 0)
            return (NodeType)i;

    return NODE;
}

class Node : public Element {

    vector<Element*> inputs;
    vector<Element*> outputs;

public:
    string name;
    NodeType type;

    Node(string id, string name, NodeType type, unsigned treeviewRow, Element* parent)
        : Element(id, treeviewRow, parent)
        , type(type)
        , name(name)
    {}

    void appendInput(Element* e) { inputs.push_back(e); }
    void appendOutput(Element* e) { outputs.push_back(e); }

    void appendIn(Element* e) { appendInput(e); };
    void appendOut(Element* e) { appendOutput(e); };

    vector<Element*>* getIn() { return &inputs; }
    vector<Element*>* getOut() { return &outputs; }

    friend ostream& operator<<(ostream& s, const Node& node)
    {
        if (node.type == NODE)
            s << node.name;
        else
            s << NodeTypeStrings[node.type];
        return s;
    }

    void print(ostream& s) const
    {

        if (type == NODE)
            printElement(s, "node id=" + id + " name=" + name);
        else
            printElement(s, "node id=" + id + " type=" + NodeTypeStrings[type]);

        printEdges(s);

        for (Element* e : inputs)
            printChild(s, "input id=" + e->id);

        for (Element* e : outputs)
            printChild(s, "output id=" + e->id);
    }

    void dot_print_element()
    {
        // cout << '\t' << "node id=" << id << " name=" << *this << '\n';
        cout << '\t' << id << "\t\t[label=\"" << name << "[" << id << "]\"]\n";
    }
};

#endif
