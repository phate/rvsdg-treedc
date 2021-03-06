#pragma once
#include "main.h"

class Region : public Element {
    vector<Element*> arguments;
    vector<Element*> results;

public:
    Region(string id, unsigned depth, Element* parent)
        : Element(id, depth, parent)
    {}

    virtual ~Region();

    void appendArgument(Element* e) { arguments.insert(arguments.end(), e); }
    void appendResult(Element* e) { results.insert(results.end(), e); }

    void appendIn(Element* e) { appendArgument(e); };
    void appendOut(Element* e) { appendResult(e); };

    vector<Element*> getIn() const { return arguments; }
    vector<Element*> getOut() const { return results; }

    void print(ostream& s) const;

    bool operator==(Element* other) const { return id == other->id; }
    bool in_arguments(Element* e) const { return find(arguments.begin(), arguments.end(), e) != arguments.end(); };
    bool in_results(Element* e) const { return find(results.begin(), results.end(), e) != results.end(); };

    void dot_print_edges(unordered_map<string, int>& node_map, int& node_map_counter, ofstream& dot_file) const;
    string getNodeTypeString(Element* e) const;
    void dot_print_arguments(unordered_map<string, int>& node_map, int& node_map_counter, ofstream& dot_file) const;
    void dot_print_results(unordered_map<string, int>& node_map, int& node_map_counter, ofstream& dot_file) const;
    void dot_print_nodes(unordered_map<string, int>& node_map, int& node_map_counter, ofstream& dot_file) const;
    void dot_print_children() const;
    void dot_print_element(unordered_map<string, int>& node_map, int& node_map_counter, ofstream& dot_file) const;
};
