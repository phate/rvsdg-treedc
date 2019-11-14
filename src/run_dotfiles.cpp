#include "run_dotfiles.h"

void run_dotfile(string f, tuple<int, int, filesystem::path>& min_max)
{
    Graph G = graph_from_dot(f.c_str());
    cout << "Graph: " << f << "\n\n";

    // cout << G << endl;
    // G.print_dotfile();
    // cout << endl;

    int hmin = minor_min_width_heuristic(G);
    cout << "minor_min_width_heuristic: " << hmin << endl;

    pair<vector<int>, int> h = min_fill_heuristic(G);
    cout << "min_fill_heuristic: " << h.second << endl;
    int local_max = h.second;

    pair<vector<int>, int> h1 = min_width_heuristic(G);
    cout << "min_width_heuristic: " << h1.second << endl;

    pair<vector<int>, int> h2 = max_cardinality_heuristic(G);
    cout << "max_cardinality_heuristic: " << h2.second << endl;

    cout << endl;

    int max = get<1>(min_max);
    if (local_max > max)
        min_max = make_tuple(hmin, local_max, f);
}

void run_dotfiles(string root)
{
    cout << "Running on: " << root << endl;

    tuple<int, int, filesystem::path> min_max = make_tuple(INT_MAX, 0, "");
    for (const auto& entry : filesystem::directory_iterator(root))
        if (entry.path().extension() == ".dot")
            run_dotfile(entry.path(), min_max);

    string fname = get<2>(min_max).filename();
    remove_if(fname.begin(), fname.end(), [](const char& c) { return c == '"'; });

    cout << "Highest treewidth: " << fname << " - min: " << get<0>(min_max) << ", max: " << get<1>(min_max) << endl;
}
