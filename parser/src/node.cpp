#include "node.h"

constexpr int NodeTypeLen = 5;
constexpr const char* NodeTypeStrings[] = { "node", "lambda", "gamma", "theta", "phi" };

Node::NodeType Node::getNodeType(const char str[])
{
    for (int i = 0; i < NodeTypeLen; ++i)
        if (strcmp(NodeTypeStrings[i], str) == 0)
            return (NodeType)i;

    return NODE;
}

const char* Node::getNodeTypeString() const
{
    return NodeTypeStrings[type];
}

// Printing the graph corresponding to the parsed input. Region and Node implements print()
void Node::print(ostream& s) const
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

// Printing dot file of graph

// the "->" string is reserved in the dot file format, so it needs to be escaped
string Node::replace_dot_arrow_name()
{
    string node = name;
    int index = 0;
    index = node.find("->", index);
    if (index != string::npos)
        node.replace(index, 2, "\\-\\>");
    return node;
}

// print node map id of the node with a corresponding label containing name/type and id
void Node::dot_print_element(unordered_map<string, int>& node_map, int& node_map_counter)
{
    string node;
    if (type == NODE)
        node = replace_dot_arrow_name();
    else
        node = NodeTypeStrings[type];

    print_label(id, node + "|" + id, node_map, node_map_counter);
}
