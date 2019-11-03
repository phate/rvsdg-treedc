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

    string name;
    NodeType type;

public:
    vector<Element*> inputs;
    vector<Element*> outputs;

    Node(string id, string name, NodeType type, unsigned treeviewRow, Element* parent)
        : Element(id, treeviewRow, parent)
        , type(type)
        , name(name)
    {}

    ~Node() {};

    void appendInput(Element* e) { inputs.push_back(e); }
    void appendOutput(Element* e) { outputs.push_back(e); }

    vector<Element*>* getIn() { return &inputs; }
    vector<Element*>* getOut() { return &outputs; }

    void print(ostream& s) const
    {
        if (type == NODE)
            printElement(s, "node id=" + id + ", name=" + name);
        else
            printElement(s, "node id=" + id + ", type=" + NodeTypeStrings[type]);

        for (Element* e : inputs)
            printChild(s, "input id=" + e->id);

        for (Element* e : outputs)
            printChild(s, "output id=" + e->id);

        printEdges(s);
    }
};

#endif
