#include "main.h"

/*
 * Tests built with the UnitTest++ framework [https://github.com/unittest-cpp/unittest-cpp]
 */

SUITE(graph)
{

    Graph G;

    TEST(opening_dotfile)
    {
        G = graph_from_dot("../dot_files/decomp.dot");
    }

    TEST(init_graph)
    {
        int n0[] = { 1 };
        CHECK_ARRAY_EQUAL(n0, G.getNode(0).getNeighbors(), 1);
        int n1[] = { 0, 2, 3 };
        CHECK_ARRAY_EQUAL(n1, G.getNode(1).getNeighbors(), 3);
        int n2[] = { 1, 4, 5 };
        CHECK_ARRAY_EQUAL(n2, G.getNode(2).getNeighbors(), 3);
        int n3[] = { 1, 5, 6 };
        CHECK_ARRAY_EQUAL(n3, G.getNode(3).getNeighbors(), 3);
        int n4[] = { 2, 5 };
        CHECK_ARRAY_EQUAL(n4, G.getNode(4).getNeighbors(), 2);
        int n5[] = { 2, 3, 4, 6 };
        CHECK_ARRAY_EQUAL(n5, G.getNode(5).getNeighbors(), 4);
        int n6[] = { 3, 5 };
        CHECK_ARRAY_EQUAL(n6, G.getNode(6).getNeighbors(), 2);
    }

    TEST(minor_min_width)
    {
        CHECK_EQUAL(2, minor_min_width_heuristic(G));
    }

    TEST(min_fill_heuristic)
    {
        pair<vector<int>, int> h = min_fill_heuristic(G);
        int n[] = { 0, 4, 6, 1, 2, 3, 5 };
        CHECK_ARRAY_EQUAL(n, h.first, h.first.size());
        CHECK_EQUAL(2, h.second);
    }

    TEST(min_width_heuristic)
    {
        pair<vector<int>, int> h = min_width_heuristic(G);
        int n[] = { 0, 1, 4, 2, 3, 5, 6 };
        CHECK_ARRAY_EQUAL(n, h.first, h.first.size());
        CHECK_EQUAL(2, h.second);
    }

    TEST(max_cardinality_heuristic)
    {
        pair<vector<int>, int> h = max_cardinality_heuristic(G);
        int n[] = { 6, 4, 5, 3, 2, 1, 0 };
        CHECK_ARRAY_EQUAL(n, h.first, h.first.size());
        CHECK_EQUAL(2, h.second);
    }

    TEST(kleinberg)
    {
        Graph T = Kleinberg(G);
        CHECK_EQUAL("cef", *T.getNode(0).getName());
        CHECK_EQUAL("bcf", *T.getNode(1).getName());
        CHECK_EQUAL("bdf", *T.getNode(2).getName());
        CHECK_EQUAL("dfg", *T.getNode(3).getName());
        CHECK_EQUAL("ab", *T.getNode(4).getName());
    }
}

SUITE(branchAndBound_K_7)
{

    Graph G;

    TEST(opening_dotfile)
    {
        G = graph_from_dot("../dot_files/k7.dot");
    }

    TEST(init_graph)
    {
        int n0[] = { 1, 2, 3, 4, 5, 6 };
        CHECK_ARRAY_EQUAL(n0, G.getNode(0).getNeighbors(), 6);
        int n1[] = { 0, 2, 3, 4, 5, 6 };
        CHECK_ARRAY_EQUAL(n1, G.getNode(1).getNeighbors(), 6);
        int n2[] = { 0, 1, 3, 4, 5, 6 };
        CHECK_ARRAY_EQUAL(n2, G.getNode(2).getNeighbors(), 6);
        int n3[] = { 0, 1, 2, 4, 5, 6 };
        CHECK_ARRAY_EQUAL(n3, G.getNode(3).getNeighbors(), 6);
        int n4[] = { 0, 1, 2, 3, 5, 6 };
        CHECK_ARRAY_EQUAL(n4, G.getNode(4).getNeighbors(), 6);
        int n5[] = { 0, 1, 2, 3, 4, 6 };
        CHECK_ARRAY_EQUAL(n5, G.getNode(5).getNeighbors(), 6);
        int n6[] = { 0, 1, 2, 3, 4, 5 };
        CHECK_ARRAY_EQUAL(n6, G.getNode(6).getNeighbors(), 6);
    }

    TEST(BB)
    {
        pair<vector<int>, int> bb = branchAndBound(G);
        int n[] = { 0, 1, 2, 3, 4, 5, 6 };
        CHECK_ARRAY_EQUAL(n, bb.first, 6);
        CHECK_EQUAL(6, bb.second);
    }
}
