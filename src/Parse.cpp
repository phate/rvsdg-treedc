#include "Parse.h"

// Parses a line in the dotfile, if it is an int => it is a node that should be appended to tokens
// otherwise, if endptr is not the end of the string, otherwise, if was a non-int in the token and we ignore it
void parse_line(string token, vector<int>& tokens)
{
    char* endptr;
    int num = strtol(token.c_str(), &endptr, 10);
    if (*endptr == 0)
        tokens.push_back(num);
}

// Parser for a subset of the dotfile format where we assume:
// - A single graph in the file
// - Nodes are integers labeled from 0 to 1
// - All edges out from a node are placed in a subgraph
// - A simple, nondirected graph
// It will otherwise ignore any labels in the graph
// Returns the graph represented as a adjacency list: each vector represents a
// node, as the first element, with each subsequent node in the vector being
// the nodes the root node has an edge to

bool in_token(string find, string token)
{
    int index = 0;
    index = token.find(find, index);
    return index != std::string::npos;
}

static const vector<string> node_line_blacklist = { "Graph", "graph", "node", "label", "//", "rank", "--", "->" };
bool node_line(string token)
{
    if (token.empty() || token[0] == '}' || token[0] == '{')
        return false;

    for (string s : node_line_blacklist)
        if (in_token(s, token))
            return false;

    return true;
}

vector<vector<int>> parse_dot(const char file_name[])
{
    ifstream file(file_name);
    if (file.fail())
        throw runtime_error("Cannot open file: " + string(file_name));

    vector<vector<int>> graph;
    string line;

    while (getline(file, line)) {

        stringstream line_stream(line);
        string token;

        line_stream >> token;

        if (!node_line(token))
            continue;

        vector<int> tokens;

        do
            if (node_line(token))
                parse_line(token, tokens);
        while (line_stream >> token);

        graph.push_back(tokens);
    }

    return graph;
}

// Returns a graph generated from the adjacency list returned by parse_dot()
// This is more versatile then generating the graph directly, and simplifies
// the process since it is easier to generate the graph when its size/number of
// nodes are known beforehand
Graph gen_graph(vector<vector<int>> adj_list)
{
    // find unique elements in adj_list
    vector<int> unique_list(adj_list.size(), -1);
    for (vector<int>& adj : adj_list)
        for (int i : adj)
            unique_list[i] = 1;

    int unique = 0;
    for (int i : unique_list)
        if (i > 0)
            unique++;

    Graph g = Graph(unique + 1);
    for (auto& n : adj_list) {
        int root = n[0];

        for (int i = 1; i < n.size(); ++i) {
            // make sure to not add duplicate edges as we assume the graph to have be simple (no parallel edges)
            if (!g.hasNeighbor(n[i], root))
                g.addNode(root, n[i]);
        }
    }

    return g;
}

// Return a graph object generated from a dot file
Graph graph_from_dot(const char dot_file[])
{
    return gen_graph(parse_dot(dot_file));
}
