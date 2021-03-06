#pragma once
#include "main.h"

class Node : public Element {

    vector<Element*> inputs;
    vector<Element*> outputs;

public:
    enum NodeType {
        NODE,
        RVSDG,
        LAMBDA,
        GAMMA,
        THETA,
        PHI,
        DELTA
    };

    string name;
    NodeType type;

    Node(string id, string name, NodeType type, unsigned treeviewRow, Element* parent)
        : Element(id, treeviewRow, parent)
        , name(name)
        , type(type)
    {}

    virtual ~Node();

    void appendInput(Element* e) { inputs.push_back(e); }
    void appendOutput(Element* e) { outputs.push_back(e); }

    void appendIn(Element* e) { appendInput(e); };
    void appendOut(Element* e) { appendOutput(e); };

    vector<Element*> getIn() const { return inputs; }
    vector<Element*> getOut() const { return outputs; }

    // Printing the graph corresponding to the parsed input. Region and Node implements print()
    friend ostream& operator<<(ostream& s, const Node& node)
    {
        if (node.type == NODE)
            s << node.name;
        else
            s << node.getNodeTypeString();
        return s;
    }

    static NodeType getNodeType(const char str[]);
    const char* getNodeTypeString() const;
    void print(ostream& s) const;
    string replace_dot_arrow_name() const;
    void dot_print_element(unordered_map<string, int>& node_map, int& node_map_counter, ofstream& dot_file) const;
};
