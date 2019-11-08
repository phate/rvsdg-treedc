#pragma once
#include "main.h"

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

    void print(ostream& s) const;

    bool operator==(Element* other) { return id == other->id; }
    bool in_arguments(Element* e) { return find(arguments.begin(), arguments.end(), e) != arguments.end(); };
    bool in_results(Element* e) { return find(results.begin(), results.end(), e) != results.end(); };

    void dot_print_edges(unordered_map<string, int>& node_map, int& node_map_counter, ofstream& dot_file);
    string getNodeTypeString(Element* e);
    void dot_print_arguments(unordered_map<string, int>& node_map, int& node_map_counter, ofstream& dot_file);
    void dot_print_results(unordered_map<string, int>& node_map, int& node_map_counter, ofstream& dot_file);
    void dot_print_nodes(unordered_map<string, int>& node_map, int& node_map_counter, ofstream& dot_file);
    void dot_print_children();
    void dot_print_element(unordered_map<string, int>& node_map, int& node_map_counter, ofstream& dot_file);
};
