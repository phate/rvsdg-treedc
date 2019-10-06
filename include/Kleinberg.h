#pragma once
#include "main.h"

/*
 * Finds a tree-decomposition from the graph G based on the algorithm in Kleinberg & Tardos chapter 10.5
 * Currently only supports the base case where all |X| < 3
 */

Graph Kleinberg(Graph G, bool debug = false);
