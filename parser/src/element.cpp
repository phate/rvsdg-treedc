#include "main.h"

Element::~Element()
{
    for (Edge* e : edges)
        delete e;

    for (Element* e : children)
        delete e;
}

void print_depth(unsigned depth)
{
    for (unsigned i = 0; i < depth; ++i)
        cout << "  ";
}

void Element::printElement(ostream& s, string str) const
{
    print_depth(depth);
    s << str << '\n';
}

void Element::printChild(ostream& s, string str) const
{
    print_depth(depth + 1);
    s << str << '\n';
}

// Prints edges of the element
// Can be used in debugging to make sure all edge connections are made
void Element::printEdges(ostream& s) const
{
    for (Edge* e : edges)
        if (e->target)
            printChild(s, "edge source=" + e->source->id + " target=" + e->target->id);
        else
            printChild(s, "edge source=" + e->source->id);
}

// Return corresponding integer id of string id, or add it to the map if it does not exist
int Element::id_from_map(string id, unordered_map<string, int>& node_map, int& node_map_counter) const
{
    if (node_map.count(id) == 0)
        node_map.emplace(id, node_map_counter++);
    return node_map[id];
}

// Print a formatted label row for the element
void Element::print_label(string id, string label_content, unordered_map<string, int>& node_map, int& node_map_counter, ostream& dot_file) const
{
    int map_id = id_from_map(id, node_map, node_map_counter);
    string label = "[label=\"{" + label_content + "}\"]";
    log('\t' << map_id << label << '\n');
}

// For each region in the rvsdg graph parsed, print a dot file corresponding to the region
void Element::dot_print() const
{
    unordered_map<string, int> node_map;
    int node_map_counter = 0;
    ofstream null_stream;
    for (Element* e : children)
        e->dot_print_element(node_map, node_map_counter, null_stream);
}
