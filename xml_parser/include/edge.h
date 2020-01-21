#pragma once
#include "main.h"

class Element;

class Edge {
public:
    Element* source;
    Element* target;

    // stores the id of a target when the corresponding
    // Element is not yet parsed and created
    string target_str;

    Edge(Element* source, string target_str)
        : source(source)
        , target(nullptr)
        , target_str(target_str)
    {}

    Edge(Element* source, Element* target)
        : source(source)
        , target(target)
        , target_str("")
    {}

    Edge()
        : Edge(nullptr, "")
    {}

    ~Edge() {}

    bool waiting_for_element(string id)
    {
        return !target && target_str == id;
    }
};
