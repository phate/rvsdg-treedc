#include "BranchAndBound.h"

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
