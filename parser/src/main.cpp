#include "element.h"
#include "node.h"
#include "region.h"

#include "pugixml.hpp"
#include <algorithm>
#include <iostream>

/*
   TYPES
   =====================
rvsdg            : ""
region           : id
structured node  : id, type
simple node      : id, name
edge             : source, target
output, input    : id
result, argument : id

region(nodes, arguments, results, edges)
simple node(inputs, outputs)
structured node(inputs, outputs, regions)
edges (argument/output) -> (input/result)
a/o is the user of exactly one i/r, i/r can be the origin of several a/o
*/

#define has(n) node->attribute(n)
#define val(n) node->attribute(n).value()
#define is_node(str) matching_node(node, str)

bool empty_str(const char str[])
{
    return str == NULL || str[0] == '\0';
}

bool matching_node(const pugi::xml_node_iterator& node, const char str[])
{
    return strcmp(node->name(), str) == 0;
}

Element* add_region(pugi::xml_node_iterator& node, int depth, Element* parent)
{
    Element* child = new Region(val(ATTR_ID), depth, parent);
    parent->appendChild(child);
    return child;
}

Element* add_structuredNode(pugi::xml_node_iterator& node, int depth, Element* parent)
{
    NodeType childType = getNodeType(val(ATTR_TYPE));
    Element* child = new Node(val(ATTR_ID), "", childType, depth, parent);
    parent->appendChild(child);
    return child;
}

Element* add_simpleNode(pugi::xml_node_iterator& node, int depth, Element* parent)
{
    Element* child = new Node(val(ATTR_ID), val(ATTR_NAME), NODE, depth, parent);
    parent->appendChild(child);
    return child;
}

void add_argument(pugi::xml_node_iterator& node, Element* parent)
{
    Element* child = new Element(val(ATTR_ID), parent);
    static_cast<Region*>(parent)->appendArgument(child);
}

void add_output(pugi::xml_node_iterator& node, Element* parent)
{
    Element* child = new Element(val(ATTR_ID), parent);
    static_cast<Node*>(parent)->appendOutput(child);
}

// since targets of edges may be parsed after the the edge itself, we have to
// check if a edge that points to the target exsists when creating a result or input
void resolve_edge_target(Element* parent, string id, Element* element);

void add_result(pugi::xml_node_iterator& node, Element* parent)
{
    Element* child = new Element(val(ATTR_ID), parent);
    static_cast<Region*>(parent)->appendResult(child);
    resolve_edge_target(parent, val(ATTR_ID), child);
}

void add_input(pugi::xml_node_iterator& node, Element* parent)
{
    Element* child = new Element(val(ATTR_ID), parent);
    static_cast<Node*>(parent)->appendInput(child);
    resolve_edge_target(parent, val(ATTR_ID), child);
}

// find the source or target node of an edge based on the string id
// searches through the parents inputs and outputs / arguments and results, and
// recurses through all the parents children (the edges siblings)
Element* find_source(Element& parent, string id)
{
    for (Element* e : *parent.getIn())
        if (e->id == id)
            return e;

    for (Element* e : *parent.getOut())
        if (e->id == id)
            return e;

    Element* source;
    for (Element* e : parent.children) {
        source = find_source(*e, id);
        if (source)
            return source;
    }

    return nullptr;
}

void add_edge(pugi::xml_node_iterator& node, Element* parent)
{
    // source and target will always be in the parent, or one of the parents childrens scope
    // the source is always set before the edges is created, so know that we will find it
    // target elements can however be set both before and after, so that we
    // have to both search for the target after creating the node (if it
    // already exists) else we use a temp value for the target id, and look for
    // the edge when creating the input or result node

    Element* source; // -> output, argument
    Element* target; // -> input, result
    Edge* edge;

    source = find_source(*parent, val(ATTR_SOURCE));
    if (source) {
        target = find_source(*parent, val(ATTR_TARGET));
        if (target)
            edge = new Edge(source, target);
        else
            edge = new Edge(source, val(ATTR_TARGET));

        parent->appendEdge(edge);
    } else
        cout << "ERROR: could not find source" << endl;
}

void resolve_edge_target(Element* parent, string id, Element* element)
{
    // search through both the edges of the parent, and the edges of the
    // parents siblings (aka. the edge list of the parents parent)
    // TODO: do we have to search the edge lists of all parent to the top of the tree?
    for (Edge* e : parent->edges)
        if (!e->target && e->target_str == id)
            e->target = element;

    for (Edge* e : parent->parent->edges)
        if (!e->target && e->target_str == id)
            e->target = element;
}

pugi::xml_document load_xml(const char source[])
{
    pugi::xml_document doc;
    pugi::xml_parse_result parse_result = doc.load_file(source);

    if (parse_result) {
        cout << "XML [" << source << "] parsed without errors\n\n";
    } else {
        cout << "XML [" << source << "] parsed with errors\n";
        cout << "Error description: " << parse_result.description() << "\n";
        cout << "Error offset: " << parse_result.offset << " (error at [..." << (source + parse_result.offset) << "]\n\n";
    }

    cout << "Load result: " << parse_result.description() << "\n\n";
    return doc;
}

Element* parseNode(pugi::xml_node_iterator& node, Element* parent, int depth)
{

    Element* child = nullptr;

    if (is_node(TAG_NODE) && !empty_str(val(ATTR_TYPE)))
        child = add_structuredNode(node, depth, parent);
    else if (is_node(TAG_NODE) && !empty_str(val(ATTR_NAME)))
        child = add_simpleNode(node, depth, parent);
    else if (is_node(TAG_REGION))
        child = add_region(node, depth, parent);
    else if (is_node(TAG_INPUT))
        add_input(node, parent);
    else if (is_node(TAG_OUTPUT))
        add_output(node, parent);
    else if (is_node(TAG_ARGUMENT))
        add_argument(node, parent);
    else if (is_node(TAG_RESULT))
        add_result(node, parent);
    else if (is_node(TAG_EDGE))
        add_edge(node, parent);

    return child;
}

void traverse(pugi::xml_node& parent, Element* parent_element, int depth)
{
    vector<Edge*> edgeList;
    for (auto node = parent.begin(); node != parent.end(); ++node) {
        print_depth(depth);

        cout << node->name();

        Element* child = parseNode(node, parent_element, depth);

        for (auto attribute = node->attributes_begin(); attribute != node->attributes_end(); ++attribute)
            cout << " " << attribute->name() << "=" << attribute->value();

        cout << endl;
        if (child)
            traverse(*node, child, depth + 1);
    }
}

int main()
{
    pugi::xml_document doc = load_xml("../../rvsdg.xml");
    pugi::xml_node rvsdg = doc.child("rvsdg");

    Element root("rvsdg", 0, nullptr);
    traverse(rvsdg, &root, 0);

    cout << endl;
    cout << endl;
    cout << root;
}
