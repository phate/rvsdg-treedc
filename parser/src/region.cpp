#include "region.h"

Region::~Region()
{
    for (Element* e : arguments)
        delete e;

    for (Element* e : results)
        delete e;
}

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
// Each edge points to a argument/result or input/output of a containing region or node
// For simple nodes this is modeled by, edges going into or out of the node and we thus print an edge to and from
// the node that the edge input/output are corresponding to, i.e. its parent
// If the edge is in the regions arguments or results, this is modeled by a single node for each input/out *and*
// a common entry/exit node for the region such that each argument is the child of the regions entry node and
// all result nodes have the regions exit node as a child
void Region::dot_print_edges(unordered_map<string, int>& node_map, int& node_map_counter, ofstream& dot_file)
{
    for (Edge* e : edges) {
        string source_id = e->source->id;
        string target_id = e->target->id;
        int entry_id = -1;
        int exit_id = -1;

        if (in_arguments(e->source))
            entry_id = id_from_map("entry_" + e->source->parent->id, node_map, node_map_counter);
        else
            source_id = e->source->parent->id;

        if (in_results(e->target))
            exit_id = id_from_map("exit_" + e->target->parent->id, node_map, node_map_counter);
        else
            target_id = e->target->parent->id;

        int source_map_id = id_from_map(source_id, node_map, node_map_counter);
        int target_map_id = id_from_map(target_id, node_map, node_map_counter);

        log_edge(source_map_id, target_map_id);

        if (entry_id >= 0)
            log_edge(entry_id, source_map_id);

        if (exit_id >= 0)
            log_edge(target_map_id, exit_id);
    }
}

// Assumes that e is an instance of Node and returns its node type string
string Region::getNodeTypeString(Element* e)
{
    return static_cast<Node*>(e)->getNodeTypeString();
}

// Prints the node map ids and corresponding labels for the nodes in the region with type/name and id.
// Arguments and results, are beeing denoted by the type the containing node, and id of the containing region,
// which is why we get the parent->parent->type and parent->id for these cases respectively

void Region::dot_print_arguments(unordered_map<string, int>& node_map, int& node_map_counter, ofstream& dot_file)
{
    if (!arguments.empty()) {
        Element* e = arguments.back();
        string id = "entry_" + e->parent->id;
        string type = getNodeTypeString(e->parent->parent);
        print_label(id, type + "-entry|" + id, node_map, node_map_counter, dot_file);
    }

    for (Element* e : arguments) {
        string id = e->id;
        string type = getNodeTypeString(e->parent->parent);
        print_label(id, type + "-argument|" + id, node_map, node_map_counter, dot_file);
    }
}

void Region::dot_print_results(unordered_map<string, int>& node_map, int& node_map_counter, ofstream& dot_file)
{
    if (!results.empty()) {
        Element* e = results.back();
        string id = "exit_" + e->parent->id;
        string type = getNodeTypeString(e->parent->parent);
        print_label(id, type + "-exit|" + id, node_map, node_map_counter, dot_file);
    }

    for (Element* e : results) {
        string id = e->id;
        string type = getNodeTypeString(e->parent->parent);
        print_label(id, type + "-result|" + id, node_map, node_map_counter, dot_file);
    }
}

// For each node child of the region: print node map id of the node with a corresponding label containing name/type and id
void Region::dot_print_nodes(unordered_map<string, int>& node_map, int& node_map_counter, ofstream& dot_file)
{
    for (Element* e : children)
        e->dot_print_element(node_map, node_map_counter, dot_file);
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
void Region::dot_print_element(unordered_map<string, int>& node_map, int& node_map_counter, ofstream& dot_file)
{
    node_map.clear();
    node_map_counter = 0;

    string graph_name = getNodeTypeString(parent) + "_region_" + id;
    constexpr const char fmt_line[] = "\tnode [shape=record]\n\n";

    dot_file.open("../dot_files/" + graph_name + ".dot");
    if (!dot_file.is_open())
        cout << "Could not open " << graph_name << endl;

    log("digraph " << graph_name << " {\n\n");
    log(fmt_line);

    dot_print_edges(node_map, node_map_counter, dot_file);

    log('\n');

    dot_print_arguments(node_map, node_map_counter, dot_file);

    dot_print_results(node_map, node_map_counter, dot_file);

    dot_print_nodes(node_map, node_map_counter, dot_file);

    log("}\n\n");
    dot_file.close();

    dot_print_children();
}
