#include "shortest_path.h"

// dijkstra - uses a simple array-based min search (no priority queue)
// this is O(V^2 + E) which is fine for adjacency list representation
// a heap-based version would be O((V+E)log V) but more complex to implement
PathResult dijkstra(Graph *g, int source) {
    int v = g->V;
    PathResult r;
    r.V = v;
    r.dist = malloc(v * sizeof(int));
    r.prev = malloc(v * sizeof(int));
    int *visited = calloc(v, sizeof(int));

    // init all distances to infinity
    for (int i = 0; i < v; i++) {
        r.dist[i] = INF;
        r.prev[i] = -1;
    }
    r.dist[source] = 0;

    clock_t start = clock();

    for (int count = 0; count < v; count++) {
        // find unvisited vertex with smallest distance
        int u = -1;
        int min_dist = INF;
        for (int i = 0; i < v; i++) {
            if (!visited[i] && r.dist[i] < min_dist) {
                min_dist = r.dist[i];
                u = i;
            }
        }

        if (u == -1) break; // remaining vertices unreachable
        visited[u] = 1;

        // relax all neighbors of u
        EdgeNodePtr edge = g->adj[u].head;
        while (edge != NULL) {
            int neighbor = edge->to;
            int new_dist = r.dist[u] + edge->weight;
            if (!visited[neighbor] && r.dist[u] != INF && new_dist < r.dist[neighbor]) {
                r.dist[neighbor] = new_dist;
                r.prev[neighbor] = u;
            }
            edge = edge->next;
        }
    }

    r.time_ms = (clock() - start) * 1000 / CLOCKS_PER_SEC;
    free(visited);
    return r;
}

// bellman-ford - relaxes all edges V-1 times
// slower than dijkstra O(VE) but handles negative weights
// also detects negative cycles
PathResult bellman_ford(Graph *g, int source) {
    int v = g->V;
    PathResult r;
    r.V = v;
    r.dist = malloc(v * sizeof(int));
    r.prev = malloc(v * sizeof(int));

    for (int i = 0; i < v; i++) {
        r.dist[i] = INF;
        r.prev[i] = -1;
    }
    r.dist[source] = 0;

    clock_t start = clock();

    // relax all edges V-1 times
    for (int iter = 0; iter < v - 1; iter++) {
        int changed = 0;
        // go through every edge in the graph
        for (int u = 0; u < v; u++) {
            if (r.dist[u] == INF) continue; // skip unreachable
            EdgeNodePtr edge = g->adj[u].head;
            while (edge != NULL) {
                int neighbor = edge->to;
                int new_dist = r.dist[u] + edge->weight;
                if (new_dist < r.dist[neighbor]) {
                    r.dist[neighbor] = new_dist;
                    r.prev[neighbor] = u;
                    changed = 1;
                }
                edge = edge->next;
            }
        }
        // early termination if nothing changed
        if (!changed) break;
    }

    r.time_ms = (clock() - start) * 1000 / CLOCKS_PER_SEC;
    return r;
}

// reconstructs and prints the path from source to dest
void print_path(PathResult *r, int source, int dest) {
    if (r->dist[dest] == INF) {
        printf("  no path from %d to %d\n", source, dest);
        return;
    }

    // build path backwards using prev array
    int path[1000];
    int len = 0;
    int current = dest;
    while (current != -1 && len < 1000) {
        path[len++] = current;
        current = r->prev[current];
    }

    printf("  %d -> %d: distance = %d, path: ", source, dest, r->dist[dest]);
    for (int i = len - 1; i >= 0; i--) {
        printf("%d", path[i]);
        if (i > 0) printf(" -> ");
    }
    printf("\n");
}

void print_all_distances(PathResult *r, int source) {
    printf("  distances from vertex %d:\n", source);
    for (int i = 0; i < r->V; i++) {
        if (r->dist[i] == INF)
            printf("    to %d: unreachable\n", i);
        else
            printf("    to %d: %d\n", i, r->dist[i]);
    }
}

void destroy_result(PathResult *r) {
    free(r->dist);
    free(r->prev);
}

void shortest_path_test() {
    printf("\n=== Shortest path algorithm tests ===\n\n");

    // small test graph
    //   0 --4-- 1
    //   |       |
    //   1       1
    //   |       |
    //   2 --2-- 1 --5-- 3 --3-- 4
    Graph g = create_graph(5);
    add_edge_undirected(&g, 0, 1, 4);
    add_edge_undirected(&g, 0, 2, 1);
    add_edge_undirected(&g, 2, 1, 2);
    add_edge_undirected(&g, 1, 3, 5);
    add_edge_undirected(&g, 3, 4, 3);

    // dijkstra from vertex 0
    printf("Dijkstra from vertex 0:\n");
    PathResult d = dijkstra(&g, 0);
    print_all_distances(&d, 0);

    // expected: 0->0=0, 0->1=3(via 2), 0->2=1, 0->3=8(via 2,1), 0->4=11
    printf("\n  expected: 0=0, 1=3, 2=1, 3=8, 4=11\n");
    int d_correct = (d.dist[0]==0 && d.dist[1]==3 && d.dist[2]==1 && d.dist[3]==8 && d.dist[4]==11);
    printf("  dijkstra Pass: %s\n", d_correct ? "YES" : "NO");

    // bellman-ford from vertex 0
    printf("\nBellman-Ford from vertex 0:\n");
    PathResult bf = bellman_ford(&g, 0);
    print_all_distances(&bf, 0);

    int bf_correct = (bf.dist[0]==0 && bf.dist[1]==3 && bf.dist[2]==1 && bf.dist[3]==8 && bf.dist[4]==11);
    printf("  bellman-ford Pass: %s\n", bf_correct ? "YES" : "NO");

    // both should give same result
    int same = 1;
    for (int i = 0; i < g.V; i++) {
        if (d.dist[i] != bf.dist[i]) { same = 0; break; }
    }
    printf("\n  Both algorithms agree: %s\n", same ? "YES" : "NO");

    // path reconstruction test
    printf("\nPath reconstruction:\n");
    print_path(&d, 0, 4);
    // expected: 0 -> 2 -> 1 -> 3 -> 4

    destroy_result(&d);
    destroy_result(&bf);
    destroy_graph(&g);

    printf("\nShortest path tests complete.\n");
}
