#pragma once
#include "main.h"

/*
 * Parser for a subset of the dotfile format [https://graphviz.gitlab.io/_pages/doc/info/lang.html]
 */

void parse_line(string token, vector<int>& tokens);
vector<vector<int>> parse_dot(const char file[]);
Graph gen_graph(vector<vector<int>> graph);
Graph graph_from_dot(const char file[]);
