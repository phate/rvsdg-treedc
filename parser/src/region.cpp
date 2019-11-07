#include "region.h"

// Printing the graph corresponding to the parsed input. Region and Node implements print()
void Region::print(ostream& s) const
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

// Printing dot file of graph

// Print each edge in the region aliased by the node map
// each edge points to a argument/result or input/output of a containing region or node
// here, this is modeled by, edges going into or out of the node and we thus print an edge to and from
// the node that the edge input/output are corresponding to, i.e. its parent
// If the edge is in the regions arguments or results, this is modeled by a single node for each
void Region::dot_print_edges(unordered_map<string, int>& node_map, int& node_map_counter)
{
    for (Edge* e : edges) {
        string source_id = e->source->parent->id;
        string target_id = e->target->parent->id;

        if (in_arguments(e->source))
            source_id = "argument_" + source_id;

        if (in_results(e->target))
            target_id = "result_" + target_id;

        int source_map_id = id_from_map(source_id, node_map, node_map_counter);
        int target_map_id = id_from_map(target_id, node_map, node_map_counter);

        cout << '\t' << source_map_id << " -> " << target_map_id << '\n';
    }
}

// Assumes that e is an instance of Node and returns its node type string
string Region::getNodeTypeString(Element* e)
{
    return static_cast<Node*>(e)->getNodeTypeString();
}

// Prints the node map ids and corresponding labels for the nodes in the region with type/name and id.
// Arguments and results, are beeing denoted by the type the containing node, and id of the containing region,
// which is why we get the parent->parent->id and parent->id for these cases respectively

void Region::dot_print_arguments(unordered_map<string, int>& node_map, int& node_map_counter)
{
    if (!arguments.empty()) {
        Element* e = arguments.back();
        string id = "argument_" + e->parent->id;
        string type = getNodeTypeString(e->parent->parent);
        print_label(id, type + "-argument|" + id, node_map, node_map_counter);
    }
}

void Region::dot_print_results(unordered_map<string, int>& node_map, int& node_map_counter)
{
    if (!results.empty()) {
        Element* e = results.back();
        string id = "result_" + e->parent->id;
        string type = getNodeTypeString(e->parent->parent);
        print_label(id, type + "-result|" + id, node_map, node_map_counter);
    }
}

void Region::dot_print_nodes(unordered_map<string, int>& node_map, int& node_map_counter)
{
    for (Element* e : children)
        e->dot_print_element(node_map, node_map_counter);
}

// For each node child of the region, recurse into it and print all its potential childen
// since the tree alternates node->region->node->region->... thil will always call the
// dot_print_element function in this file and we will recurse the tree
void Region::dot_print_children()
{
    for (Element* e : children)
        e->dot_print();
}

// Print one region in the rvsdg as a graph in the dot file format, then recurse into
// potential sub-regions of this region
// all id's are aliased to an integer from 0 in rising order as they are discovered.
// the nodemap contains the mapping from string id to integer id and the map counter
// contains the value of the last assigned node
void Region::dot_print_element(unordered_map<string, int>& node_map, int& node_map_counter)
{
    node_map.clear();
    node_map_counter = 0;

    cout << "digraph " << getNodeTypeString(parent);
    cout << "_region_" << id << " {\n\n";
    cout << "\tnode [shape=record]\n\n";

    dot_print_edges(node_map, node_map_counter);

    cout << '\n';

    dot_print_arguments(node_map, node_map_counter);

    dot_print_results(node_map, node_map_counter);

    dot_print_nodes(node_map, node_map_counter);

    cout << "}\n\n";

    dot_print_children();
}
