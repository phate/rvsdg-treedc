#ifndef REGION_H
#define REGION_H

#include "element.h"
#include "node.h"
#include <algorithm>

class Region : public Element {
    vector<Element*> arguments;
    vector<Element*> results;

public:
    Region(string id, unsigned treeviewRow, Element* parent)
        : Element(id, treeviewRow, parent)
    {}

    void appendArgument(Element* e) { arguments.insert(arguments.end(), e); }
    void appendResult(Element* e) { results.insert(results.end(), e); }

    void appendIn(Element* e) { appendArgument(e); };
    void appendOut(Element* e) { appendResult(e); };

    vector<Element*>* getIn() { return &arguments; }
    vector<Element*>* getOut() { return &results; }

    void print(ostream& s) const
    {
        printElement(s, "region id=" + id);
        printEdges(s);
        for (Element* e : arguments)
            printChild(s, "argument id=" + e->id);

        for (Element* e : results)
            printChild(s, "result id=" + e->id);

        for (Element* e : children) {
            e->print(s);
            s << *e;
        }
    }

    bool operator==(Element* other) { return id == other->id; }
    bool in_arguments(Element* element) { return find(arguments.begin(), arguments.end(), element) != arguments.end(); }
    bool in_results(Element* element) { return find(results.begin(), results.end(), element) != results.end(); }

    void dot_print_element()
    {
        cout << "digraph " << NodeTypeStrings[static_cast<Node*>(parent)->type];
        cout << "_region_id_" << id << " {\n\n";

        // for (Edge* e : edges) {
        //     cout << '\t';
        //     if (in_arguments(e->source))
        //         cout << "argument[" << NodeTypeStrings[static_cast<Node*>(e->source->parent->parent)->type] << "-" << e->source->parent->id << "]";
        //     else
        //         cout << *static_cast<Node*>(e->source->parent) << "[" << e->source->parent->id << "]";
        //
        //     cout << " -> ";
        //     if (in_results(e->target))
        //         cout << "result[" << NodeTypeStrings[static_cast<Node*>(e->target->parent->parent)->type] << "-" << e->target->parent->id << "]";
        //     else
        //         cout << *static_cast<Node*>(e->target->parent) << "[" << e->target->parent->id << "]";
        //
        //     cout << "\n";
        // }
        //
        // cout << endl;

        for (Edge* e : edges) {
            cout << '\t';
            if (in_arguments(e->source))
                cout << "argument_" << e->source->parent->id;
            else
                cout << e->source->parent->id;

            cout << " -> ";
            if (in_results(e->target))
                cout << "result_" << e->target->parent->id;
            else
                cout << e->target->parent->id;

            cout << '\n';
        }

        cout << '\n';

        for (Element* e : arguments) {
            cout << "\targument_" << e->parent->id << "[label=\"argument[" << NodeTypeStrings[static_cast<Node*>(e->parent->parent)->type] << "-" << e->parent->id << "]\"]\n";
            break;
        }

        for (Element* e : results) {
            cout << "\tresult_" << e->parent->id << "\t[label=\"result[" << NodeTypeStrings[static_cast<Node*>(e->parent->parent)->type] << "-" << e->parent->id << "]\"]\n";
            break;
        }

        for (Element* e : children)
            e->dot_print_element();

        cout << "}\n\n";

        for (Element* e : children)
            e->dot_print();
    }
};

#endif
