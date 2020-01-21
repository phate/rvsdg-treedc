#pragma once
#include "main.h"

class Edge;

///////////////////////////////////////////////////////////////////////////////
// XML defines

#define ROOT_NODE "rvsdg"

#define TAG_NODE "node"
#define TAG_REGION "region"
#define TAG_EDGE "edge"
#define TAG_INPUT "input"
#define TAG_OUTPUT "output"
#define TAG_ARGUMENT "argument"
#define TAG_RESULT "result"

#define ATTR_ID "id"
#define ATTR_SOURCE "source"
#define ATTR_TARGET "target"
#define ATTR_NAME "name"
#define ATTR_TYPE "type"

///////////////////////////////////////////////////////////////////////////////

// Write output to both stdout and file
#define log(s)         \
    do {               \
        if (debug)     \
            cout << s; \
        dot_file << s; \
    } while (0)
#define log_edge(e1, e2) \
    log('\t' << e1 << " -> " << e2 << '\n')

void print_depth(unsigned depth);

class Element {

public:
    string id;
    Element* parent;
    vector<Element*> children;
    vector<Edge*> edges;
    unsigned depth;

    Element(string id, unsigned depth, Element* parent)
        : id(id)
        , parent(parent)
        , depth(depth)
    {}

    Element(string id, Element* parent)
        : Element(id, 0, parent)
    {}

    Element(string id)
        : Element(id, nullptr)
    {}

    virtual ~Element();

    void appendEdge(Edge* e) { edges.push_back(e); }
    void appendChild(Element* e) { children.push_back(e); }

    virtual vector<Element*> getIn() const { return {}; }
    virtual vector<Element*> getOut() const { return {}; }

    virtual void appendIn(Element*) {};
    virtual void appendOut(Element*) {};

    // Printing the graph corresponding to the parsed input. Region and Node implements print()
    void xml_print(ostream& s = cout) const
    {
        for (Element* e : children)
            e->print(s);
    }
    friend ostream& operator<<(ostream& s, const Element& element)
    {
        element.xml_print(s);
        return s;
    }
    virtual void print(ostream& s) const { s << " id=" << id << " "; };
    void printElement(ostream& s, string str) const;
    void printChild(ostream& s, string str) const;

    // Printing dot file of graph. Region and Node implements dot_print_element()
    virtual void dot_print_element(unordered_map<string, int>&, int&, ofstream&) const {}
    void dot_print() const;
    void printEdges(ostream& s) const;
    int id_from_map(string id, unordered_map<string, int>& node_map, int& node_map_counter) const;
    void print_label(string id, string label_content, unordered_map<string, int>& node_map, int& node_map_counter, ostream& dot_file) const;
};
