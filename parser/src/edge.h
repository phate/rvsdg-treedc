#ifndef EDGE_H
#define EDGE_H

#include <string>
using namespace std;

class Element;

class Edge {
public:
    Element* source;
    Element* target;
    string target_str;

    Edge(Element* source, string target_str)
        : source(source)
        , target_str(target_str)
        , target(nullptr)
    {}

    Edge(Element* source, Element* target)
        : source(source)
        , target(target)
        , target_str("")
    {}

    Edge()
        : Edge(nullptr, "")
    {}
};

#endif
