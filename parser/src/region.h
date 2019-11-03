#ifndef REGION_H
#define REGION_H

#include "element.h"

class Region : public Element {

public:
    vector<Element*> arguments;
    vector<Element*> results;

    Region(string id, unsigned treeviewRow, Element* parent)
        : Element(id, treeviewRow, parent)
    {}

    ~Region() {};

    void appendArgument(Element* e) { arguments.insert(arguments.end(), e); }
    void appendResult(Element* e) { results.insert(results.end(), e); }

    vector<Element*>* getIn() { return &arguments; }
    vector<Element*>* getOut() { return &results; }

    void print(ostream& s) const
    {
        printElement(s, "region id=" + id);
        for (Element* e : arguments)
            printChild(s, "argument id=" + e->id);

        for (Element* e : results)
            printChild(s, "result id=" + e->id);

        printEdges(s);

        for (Element* e : children) {
            e->print(s);
            s << *e;
        }
    }
};

#endif
