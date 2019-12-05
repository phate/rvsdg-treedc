#pragma once
#include "main.h"

/*
 * Implementation of heuristics for upper and lower bound on treewidth.
 *
 * Heuristics run in polynomial in size of the graph and can form important candidates that could be used to place good upper bounds on the branch and bound scheme.
 * The lower bound heuristics are also used to construct a perfect elimination ordering of the graph.
 *
 * Implementation based on Googate & Decher [https://arxiv.org/abs/1207.4109], and a reference python implementation in the D-Wave-NetworkX library
 * [https://docs.ocean.dwavesys.com/projects/dwave-networkx/en/latest/_modules/dwave_networkx/algorithms/elimination_ordering.html#min_width_heuristic]
 */

pair<vector<int>, int> min_fill_heuristic(Graph g);
pair<vector<int>, int> min_width_heuristic(Graph g);
pair<vector<int>, int> max_cardinality_heuristic(Graph g);
int minor_min_width_heuristic(Graph g);

pair<vector<int>, int> BB(Graph& G, vector<int> x, int g, int f, pair<vector<int>, int> best_found);
pair<vector<int>, int> branchAndBound(Graph G);
