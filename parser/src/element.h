#ifndef ELEMENT_H
#define ELEMENT_H

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

    ~Element();

    void appendEdge(Edge* e) { edges.push_back(e); }
    void appendChild(Element* e) { children.push_back(e); }

    virtual vector<Element*>* getIn() { return nullptr; }
    virtual vector<Element*>* getOut() { return nullptr; }

    virtual void appendIn(Element* e) {}
    virtual void appendOut(Element* e) {}

    void printElement(ostream& s, string str) const
    {
        print_depth(depth);
        s << str << '\n';
    }

    void printChild(ostream& s, string str) const
    {
        print_depth(depth + 1);
        s << str << '\n';
    }

    void printEdges(ostream& s) const
    {
        for (Edge* e : edges)
            if (e->target)
                printChild(s, "edge source=" + e->source->id + " target=" + e->target->id);
            else
                printChild(s, "edge source=" + e->source->id);
    }

    virtual void print(ostream& s) const { s << " id=" << id << " "; };

    friend ostream& operator<<(ostream& s, const Element& element)
    {
        for (Element* e : element.children)
            e->print(s);
        return s;
    }

    virtual void dot_print_element() {}

    void dot_print()
    {
        for (Element* e : children)
            e->dot_print_element();
    }
};

#endif
