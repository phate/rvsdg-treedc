#include "run_dotfiles.h"

typedef tuple<int, int, filesystem::path> min_max;

string get_filename(filesystem::path& m)
{
    string fname = m.filename();
    remove_if(fname.begin(), fname.end(), [](const char& c) { return c == '"'; });
    fname.erase(fname.find(".dot"), 4);
    return fname;
}

void run_dotfile(filesystem::path f, min_max& max, min_max& min, min_max& gap)
{
    Graph G = graph_from_dot(f.c_str());
    cout << "Graph: " << get_filename(f) << "\n";
    cout << "Number of nodes: " << G.size() << "\n\n";

    // cout << G << endl;
    // G.print_dotfile();
    // cout << endl;

    int hmin = minor_min_width_heuristic(G);
    cout << "minor_min_width_heuristic: " << hmin << endl;

    pair<vector<int>, int> h = min_fill_heuristic(G);
    cout << "min_fill_heuristic: " << h.second << endl;
    int hmax = h.second;

    pair<vector<int>, int> h1 = min_width_heuristic(G);
    cout << "min_width_heuristic: " << h1.second << endl;

    pair<vector<int>, int> h2 = max_cardinality_heuristic(G);
    cout << "max_cardinality_heuristic: " << h2.second << endl;

    cout << endl;

    if (hmax > get<1>(max))
        max = make_tuple(hmin, hmax, f);

    if (hmin < get<0>(min))
        min = make_tuple(hmin, hmax, f);

    if (hmax - hmin > get<1>(gap) - get<0>(gap))
        gap = make_tuple(hmin, hmax, f);
}

// Run the heurisitc algorithms and each dotfile in the xml_parse/dotfile directory.
// Report the highest, lowest and largest treewidth gap for each graph.

void run_dotfiles(string root)
{
    cout << "Running on: " << root << "\n\n";

    min_max max = make_tuple(0, 0, "");
    min_max min = make_tuple(INT_MAX, INT_MAX, "");
    min_max gap = make_tuple(INT_MAX, 0, "");

    for (const auto& entry : filesystem::directory_iterator(root))
        if (entry.path().extension() == ".dot")
            run_dotfile(entry.path(), max, min, gap);

    if (get<2>(max).string().size() == 0)
        cout << "No dotfiles tested" << endl;
    else {
        cout << "Highest treewidth: " << get_filename(get<2>(max)) << " - min: " << get<0>(max) << ", max: " << get<1>(max) << endl;
        cout << "Lowest treewidth:  " << get_filename(get<2>(min)) << " - min: " << get<0>(min) << ", max: " << get<1>(min) << endl;
        cout << "Largest gap:       " << get_filename(get<2>(gap)) << " - min: " << get<0>(gap) << ", max: " << get<1>(gap) << endl;
    }
}
