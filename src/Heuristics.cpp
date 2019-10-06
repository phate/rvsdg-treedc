#include "Heuristics.h"

// Get node with the least value based the comparison function passed
int Graph::min_node(vector<int> nodes, int (Graph::*min_f)(int) const) const
{
    auto min_it = min_element(nodes.begin(), nodes.end(),
        [this, min_f](int p1, int p2) { return (this->*min_f)(p1) < (this->*min_f)(p2); });
    return *min_it;
}

// A Node v is simplistic if its set of neighbors in G induces a clique
// The node indicated by label is simplified by going through its set of existing neighbors,
// and if neighbor a, does not have an edge to neighbor b, add an edge between them by adding
// b as a neighbor for a
void Graph::simplify(int label)
{
    vector<Node*> neighbors = getNeighbors(label);

    for (Node* i : neighbors)
        for (Node* j : neighbors)
            if (*i != *j && !i->hasNeighbor(j->getLabel()))
                i->addNode(*j);
}

// A node is eliminate by making it simplistic, and then removing it from the graph
void Graph::eliminate(int label)
{
    simplify(label);
    removeNode(label);
}

// Get the degree of the node denoted by the label (its number of neighbors)
int Graph::getDegree(int label) const
{
    return getNeighborsConst(label).size();
}

// Computes an upper bound on the treewidth of graph G based on the min-fill heuristic
// The algorithm alters G, so we take a copy of it
pair<vector<int>, int> min_fill_heuristic(Graph g)
{
    return g.min_fill_heuristic();
}

// Return the perfect elimination ordering and the upper bound treewidth
// Based on ordering of the nodes that adds the fewest number of edges when eliminated from the graph.
pair<vector<int>, int> Graph::min_fill_heuristic()
{
    vector<int> ordering(size());
    int upper_bound = 0;
    for (int i = 0; i < size(); ++i) {

        // get the node that adds the fewest number of edges when eliminated from the graph.
        int v = minClique();

        // by lemma 2.8, given the perfect elimination ordering, tw is given by:
        // max{neighbors of v_i which is in the ordering at position j>i}
        upper_bound = max(upper_bound, getDegree(v));

        // make v simplistic by making its neighborhood a clique and then remove the node
        eliminate(v);
        ordering[i] = v;
    }

    return make_pair(ordering, upper_bound);
}

// Returns the label of the node with the lowest value calculated on missingFromClique()
int Graph::minClique() const
{
    return min_node(activeNodes(), &Graph::missingFromClique);
}

// Counts how many edges needs to be added to make the node denoted by label simplistic
// That is, how many edges needs to be added to make the nodes set of neighbors a clique
int Graph::missingFromClique(int label) const
{
    vector<const Node*> neighbors = getNeighborsConst(label);
    int not_neighbors = 0;

    for (const Node* i : neighbors)
        for (const Node* j : neighbors)
            if (*i != *j && !i->hasNeighbor(j->getLabel()))
                not_neighbors++;

    return not_neighbors;
}

// Computes an upper bound on the treewidth of graph G based on the min-width heuristic for the elimination ordering.
// The algorithm alters G, so we take a copy of it
pair<vector<int>, int> min_width_heuristic(Graph g)
{
    return g.min_width_heuristic();
}

// Return the perfect elimination ordering and the upper bound treewidth
// Based on ordering of the nodes that have the lowest degree.
pair<vector<int>, int> Graph::min_width_heuristic()
{
    vector<int> ordering(size());
    int upper_bound = 0;
    for (int i = 0; i < size(); ++i) {

        // get the node with the smallest degree
        int v = minDegree();

        // by lemma 2.8, given the perfect elimination ordering, tw is given by:
        // max{neighbors of v_i which is in the ordering at position j>i}
        upper_bound = max(upper_bound, getDegree(v));

        // make v simplistic by making its neighborhood a clique and then remove the node
        eliminate(v);
        ordering[i] = v;
    }

    return make_pair(ordering, upper_bound);
}

// Returns the label of the node with the lowest degree
int Graph::minDegree() const
{
    return min_node(activeNodes(), &Graph::getDegree);
}

// Computes an upper bound on the treewidth of graph G based on the max-cardinality heuristic for the elimination ordering.
// The algorithm alters G, so we take a copy of it
pair<vector<int>, int> max_cardinality_heuristic(Graph g)
{
    return g.max_cardinality_heuristic();
}

// Return the perfect elimination ordering and the upper bound treewidth
// Based on ordering of the nodes with the most neighbors already ordered
pair<vector<int>, int> Graph::max_cardinality_heuristic()
{
    vector<int> ordering(size(), -1);
    int upper_bound = 0;

    // Ordering the vertices of a graph from n to 1, starting by putting the first node at position n
    ordering[size() - 1] = 0;
    for (int i = size() - 2; i >= 0; --i)
        // Get the node with the most labeled neighbors.
        ordering[i] = maxLabeledNeighbors(ordering);

    // Find the upper bound treewidth by iterating the ordering
    for (int v : ordering) {
        // by lemma 2.8, given the perfect elimination ordering, tw is given by:
        // max{neighbors of v_i which is in the ordering at position j>i}
        upper_bound = max(upper_bound, getDegree(v));

        // make v simplistic by making its neighborhood a clique and then remove the node
        eliminate(v);
    }

    return make_pair(ordering, upper_bound);
}

// Returns the label of the node with the most labeled neighbors. That is, the node with the most neighbors already in the ordering.
int Graph::maxLabeledNeighbors(vector<int> ordering) const
{
    int max_labeled_element = 0, max_labeled = 0;
    for (int i = 0; i < ordering.size(); ++i) {
        if (!in(ordering, i)) {

            int labeled = 0;
            vector<int> neighbors = getNode(i)->getNeighbors();
            for (int n : neighbors)
                if (in(ordering, n))
                    labeled++;

            if (labeled > max_labeled) {
                max_labeled = labeled;
                max_labeled_element = i;
            }
        }
    }

    return max_labeled_element;
}

// Computes a lower bound on the treewidth of graph G based on the minor-min-width heuristic
// The algorithm alters G, so we take a copy of it
int minor_min_width_heuristic(Graph g)
{
    return g.minor_min_width_heuristic();
}

// Return the lower bound on treewidth for the graph
// Based on the idea that if in a min-width ordering some vertex v has en edge with lb vertices ordered below it in graph G, then the treewidth of the graph is at least lb. Improved on this again by using the theorem which states that the treewidth of a graph is never less han the treewidth of its minor. Thus we can contract edges for each node we find to create smaller and smaller minors of the original graph.
int Graph::minor_min_width_heuristic()
{
    int lower_bound = 0;
    for (int i = 0; i < size(); ++i) {

        // get the node with the smallest degree
        int v = minDegree();

        vector<int> neighbors = getNode(v).getNeighbors();

        // v is a singleton, we can delete it
        if (neighbors.size() == 0) {
            removeNode(v);
            continue;
        }

        // find u in N(v) such that the degree of u is minimum in N(v)
        int u = min_node(neighbors, &Graph::getDegree);

        // update lower bound
        lower_bound = max(lower_bound, getDegree(v));

        // contract the edge between u, v by removing v, and adding all its neighbors to u. The resulting graph from this is a minor of the existing graph
        removeNode(v);
        for (int n : neighbors)
            if (n != u && !hasNeighbor(u, n))
                addNode(u, n);
    }

    return lower_bound;
}

pair<vector<int>, int> branchAndBound(Graph G)
{
    // Partial order
    vector<int> x;

    // f is the lower bound lower bound on the treewidth
    int f = minor_min_width_heuristic(G);

    // g is the width of the ordering x along the path from the root
    int g = 0;

    // Upper bound ub is set to the bound computed by the min-fill algorithm
    auto best_found = min_fill_heuristic(G);
    vector<int> order = best_found.first;
    int ub = best_found.second;

    // if f < ub, i.e. the lower bound equals the treewidth returned by the min-fill algorithm, it is returned as the optimal solution. Othrwise we initialize the best solution found so far to the min-fill solution and start a branch and bond search for a better solution.
    if (f < ub)
        best_found = BB(G, x, g, f, best_found);

    return best_found;
}

pair<vector<int>, int> BB(Graph& G, vector<int> x, int g, int f, pair<vector<int>, int> best_found)
{

    vector<int> order = best_found.first;
    int ub = best_found.second;

    // - once a partial solution is found whose lower bound on treewidth is greater then ub, we prune the branc of the seach.
    // - othereise, if we find a complete ordering that is better than the best so war, we update he bes solution found so far( stored in ub)
    // - performs a search in the space of perfect elimination ordering of vertices of G.

    if (G.size() < 2)
        return make_pair(best_found.first, min(ub, f));

    else
        for (int i = 0; i < G.size(); ++i) {
            Graph G_s = Graph(G);
            vector<int> x_s = x;

            // the succssors of a state s can be obtained by eliminating a vertex v og G.
            G_s.eliminate(i);
            x_s.push_back(i);

            int g_m = max(g, G.getDegree(i));
            int f_m = max(g_m, minor_min_width_heuristic(G_s));
            if (f_m < ub) {
                best_found = BB(G_s, x_s, g_m, f_m, best_found);
                ub = best_found.second;
            }
        }

    return best_found;
}
