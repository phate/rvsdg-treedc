#pragma once
#include "main.h"

/*
 * IN PROGRESS implementation of Branch and Bound algorithm from quickBB
 *
 * Implementation based on Googate & Decher [https://arxiv.org/abs/1207.4109], and a reference python implementation in the D-Wave-NetworkX library
 * [https://docs.ocean.dwavesys.com/projects/dwave-networkx/en/latest/_modules/dwave_networkx/algorithms/elimination_ordering.html#min_width_heuristic]
 */

pair<vector<int>, int> branchAndBound(Graph G);
pair<vector<int>, int> BB(Graph& G, vector<int> x, int g, int f, pair<vector<int>, int> best_found);
