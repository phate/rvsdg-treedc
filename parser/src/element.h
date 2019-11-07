#pragma once
#include "main.h"

class Edge;

///////////////////////////////////////////////////////////////////////////////
// XML defines

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

#include "edge.h"
#include <iostream>
#include <string>
#include <vector>

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
        , depth(depth)
        , parent(parent)
    {}

    Element(string id, Element* parent)
        : Element(id, 0, parent)
    {}

    Element(string id)
        : Element(id, nullptr)
    {}

    ~Element();

    void appendEdge(Edge* e) { edges.push_back(e); }
    void appendChild(Element* e) { children.push_back(e); }

    virtual vector<Element*>* getIn() { return nullptr; }
    virtual vector<Element*>* getOut() { return nullptr; }

    virtual void appendIn(Element* e) {}
    virtual void appendOut(Element* e) {}

    // Printing the graph corresponding to the parsed input. Region and Node implements print()
    friend ostream& operator<<(ostream& s, const Element& element)
    {
        for (Element* e : element.children)
            e->print(s);
        return s;
    }
    virtual void print(ostream& s) const { s << " id=" << id << " "; };
    void printElement(ostream& s, string str) const;
    void printChild(ostream& s, string str) const;

    // Printing dot file of graph. Region and Node implements dot_print_element()
    virtual void dot_print_element(unordered_map<string, int>& node_map, int& node_map_counter) {}
    void dot_print();
    void printEdges(ostream& s) const;
    int id_from_map(string id, unordered_map<string, int>& node_map, int& node_map_counter);
    void print_label(string id, string label_content, unordered_map<string, int>& node_map, int& node_map_counter);
};
