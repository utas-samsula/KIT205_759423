/*
 * KIT205 Assignment 2 - Social Network Shortest Path Analysis
 * Student: Samsul Alam (759423)
 * GitHub: github.com/utas-samsula/KIT205_759423
 *
 * Problem: Finding shortest paths in social networks using graph algorithms
 * Part 1: Dijkstra's algorithm on weighted social network graph
 * Part 2: Comparing Dijkstra vs Bellman-Ford performance
 *
 * Resources:
 *   - KIT205 Week 8 tutorial (graph adjacency list)
 *   - KIT205 lecture notes on graph algorithms
 *   - Claude AI used for scaffolding, all code tested personally
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "graph.h"
#include "shortest_path.h"

// --- performance comparison ---

void run_comparison() {
    printf("\n====== Performance Comparison: Dijkstra vs Bellman-Ford ======\n\n");
    printf("Testing on random social network graphs with positive weights\n");
    printf("Measuring time to compute single-source shortest path from vertex 0\n\n");

    srand(42); // fixed seed so results are reproducible

    printf("%-12s %-12s %-18s %-18s %-10s\n",
           "Vertices", "Edges", "Dijkstra (ms)", "B-Ford (ms)", "Speedup");
    printf("----------------------------------------------------------------------\n");

    // test with increasing graph sizes
    int test_v[] = {100, 500, 1000, 2000, 5000};
    int n_tests = 5;

    for (int t = 0; t < n_tests; t++) {
        int v = test_v[t];
        int e = v * 5; // avg degree ~10 which is realistic for social networks

        Graph g = generate_random_graph(v, e, 100);

        // run both algorithms multiple times for accuracy
        int runs = 3;
        long d_total = 0, bf_total = 0;

        for (int r = 0; r < runs; r++) {
            PathResult d = dijkstra(&g, 0);
            PathResult bf = bellman_ford(&g, 0);
            d_total += d.time_ms;
            bf_total += bf.time_ms;

            // verify both give same answer (sanity check)
            if (r == 0) {
                int agree = 1;
                for (int i = 0; i < v; i++) {
                    if (d.dist[i] != bf.dist[i]) { agree = 0; break; }
                }
                if (!agree) printf("WARNING: algorithms disagree for V=%d\n", v);
            }
            destroy_result(&d);
            destroy_result(&bf);
        }

        long d_avg = d_total / runs;
        long bf_avg = bf_total / runs;

        // speedup calculation
        if (d_avg == 0 && bf_avg == 0)
            printf("%-12d %-12d %-18ld %-18ld %-10s\n", v, g.E, d_avg, bf_avg, "-");
        else if (d_avg == 0)
            printf("%-12d %-12d %-18ld %-18ld %-10s\n", v, g.E, d_avg, bf_avg, "D fast");
        else {
            double speedup = (double)bf_avg / (double)d_avg;
            printf("%-12d %-12d %-18ld %-18ld %-10.1fx\n", v, g.E, d_avg, bf_avg, speedup);
        }

        destroy_graph(&g);
    }

    printf("\n--- Varying edge density (fixed V=1000) ---\n\n");
    printf("%-12s %-12s %-18s %-18s\n", "Edges", "Avg degree", "Dijkstra (ms)", "B-Ford (ms)");
    printf("--------------------------------------------------------------\n");

    int edge_counts[] = {2000, 5000, 10000, 20000, 50000};
    for (int t = 0; t < 5; t++) {
        int e = edge_counts[t];
        Graph g = generate_random_graph(1000, e, 100);

        PathResult d = dijkstra(&g, 0);
        PathResult bf = bellman_ford(&g, 0);

        printf("%-12d %-12d %-18ld %-18ld\n", g.E, g.E / 1000, d.time_ms, bf.time_ms);

        destroy_result(&d);
        destroy_result(&bf);
        destroy_graph(&g);
    }

    printf("\nTheoretical analysis:\n");
    printf("  Dijkstra (array): O(V^2 + E)\n");
    printf("  Bellman-Ford:     O(V * E)\n");
    printf("  For sparse graphs (E ~ V), Dijkstra is O(V^2) vs B-Ford O(V^2) - similar\n");
    printf("  For dense graphs (E ~ V^2), Dijkstra is O(V^2) vs B-Ford O(V^3) - D wins\n");
}

// --- demo on small social network ---

void run_demo() {
    printf("\n====== Demo: Social Network Shortest Path ======\n\n");

    // a small social network where vertices are people
    // edge weights = "distance" (inverse of closeness)
    // lower weight = closer friends
    printf("Network: 8 people, edges = interaction distance (lower = closer)\n\n");
    printf("  Alice(0) -- 2 -- Bob(1) -- 6 -- Eve(4)\n");
    printf("    |                |                |\n");
    printf("    4                1                3\n");
    printf("    |                |                |\n");
    printf("  Charlie(2) - 3 - Dave(3) -- 1 -- Frank(5)\n");
    printf("    |                                 |\n");
    printf("    7                                 2\n");
    printf("    |                                 |\n");
    printf("  Grace(6) -------- 5 ------------ Helen(7)\n\n");

    Graph g = create_graph(8);
    add_edge_undirected(&g, 0, 1, 2); // alice-bob
    add_edge_undirected(&g, 0, 2, 4); // alice-charlie
    add_edge_undirected(&g, 1, 3, 1); // bob-dave
    add_edge_undirected(&g, 1, 4, 6); // bob-eve
    add_edge_undirected(&g, 2, 3, 3); // charlie-dave
    add_edge_undirected(&g, 2, 6, 7); // charlie-grace
    add_edge_undirected(&g, 3, 5, 1); // dave-frank
    add_edge_undirected(&g, 4, 5, 3); // eve-frank
    add_edge_undirected(&g, 5, 7, 2); // frank-helen
    add_edge_undirected(&g, 6, 7, 5); // grace-helen

    printf("Shortest paths from Alice (vertex 0) using Dijkstra:\n");
    PathResult d = dijkstra(&g, 0);
    print_all_distances(&d, 0);

    printf("\nHow Alice reaches each person:\n");
    for (int i = 1; i < 8; i++)
        print_path(&d, 0, i);

    printf("\nSame computation using Bellman-Ford:\n");
    PathResult bf = bellman_ford(&g, 0);
    int agree = 1;
    for (int i = 0; i < 8; i++)
        if (d.dist[i] != bf.dist[i]) agree = 0;
    printf("  Both algorithms agree: %s\n", agree ? "YES" : "NO");

    printf("\nDijkstra time: %ld ms, Bellman-Ford time: %ld ms\n", d.time_ms, bf.time_ms);

    destroy_result(&d);
    destroy_result(&bf);
    destroy_graph(&g);
}

// --- file-based test ---

void run_file_test() {
    printf("\n====== File-based graph test ======\n\n");

    Graph g = load_graph_weighted("social_network.txt");
    if (g.V == 0) {
        printf("skipping file test (file not found)\n");
        return;
    }

    printf("loaded graph: %d vertices, %d edges\n", g.V, g.E);

    PathResult d = dijkstra(&g, 0);
    PathResult bf = bellman_ford(&g, 0);

    printf("\nDijkstra from vertex 0:\n");
    print_all_distances(&d, 0);

    // verify both agree
    int agree = 1;
    for (int i = 0; i < g.V; i++)
        if (d.dist[i] != bf.dist[i]) agree = 0;
    printf("\nBoth agree: %s\n", agree ? "YES" : "NO");
    printf("Dijkstra: %ld ms, Bellman-Ford: %ld ms\n", d.time_ms, bf.time_ms);

    destroy_result(&d);
    destroy_result(&bf);
    destroy_graph(&g);
}

int main() {
    printf("KIT205 Assignment 2 - Social Network Shortest Path\n");
    printf("Student: Samsul Alam (759423)\n");
    printf("Comparing: Dijkstra vs Bellman-Ford\n");
    printf("================================================\n");

    srand(42);

    // part 1: unit tests
    graph_test();
    shortest_path_test();

    // demo with named social network
    run_demo();

    // file-based test
    run_file_test();

    // part 2: performance comparison
    run_comparison();

    printf("\n================================================\n");
    printf("All tests and evaluation complete.\n");

    return 0;
}
