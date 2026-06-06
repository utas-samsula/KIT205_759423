#include "graph.h"
#include <string.h>

Graph create_graph(int v) {
    Graph g;
    g.V = v;
    g.E = 0;
    g.adj = malloc(v * sizeof(EdgeList));
    for (int i = 0; i < v; i++) g.adj[i].head = NULL;
    return g;
}

void add_edge(Graph *g, int from, int to, int weight) {
    EdgeNodePtr n = malloc(sizeof *n);
    n->to = to;
    n->weight = weight;
    n->next = g->adj[from].head;
    g->adj[from].head = n;
    g->E++;
}

// for social networks edges go both ways
void add_edge_undirected(Graph *g, int u, int v, int weight) {
    add_edge(g, u, v, weight);
    add_edge(g, v, u, weight);
}

void print_graph(Graph *g) {
    for (int i = 0; i < g->V; i++) {
        printf("  %d -> ", i);
        EdgeNodePtr c = g->adj[i].head;
        while (c) {
            printf("(%d,w=%d) ", c->to, c->weight);
            c = c->next;
        }
        printf("\n");
    }
}

int out_degree(Graph *g, int v) {
    int count = 0;
    EdgeNodePtr c = g->adj[v].head;
    while (c) { count++; c = c->next; }
    return count;
}

void destroy_graph(Graph *g) {
    for (int i = 0; i < g->V; i++) {
        EdgeNodePtr c = g->adj[i].head;
        while (c) {
            EdgeNodePtr tmp = c;
            c = c->next;
            free(tmp);
        }
    }
    free(g->adj);
    g->V = 0; g->E = 0;
}

// reads: first line = V, then lines of from,to,weight
Graph load_graph_weighted(const char *filename) {
    FILE *f = fopen(filename, "r");
    if (!f) { printf("cannot open %s\n", filename); Graph e = {0,0,NULL}; return e; }
    int v; fscanf(f, "%d", &v);
    Graph g = create_graph(v);
    int from, to, w;
    while (fscanf(f, "%d,%d,%d", &from, &to, &w) == 3)
        add_edge(&g, from, to, w);
    fclose(f);
    return g;
}

// reads: first line = V, then lines of from,to (weight = 1)
Graph load_graph_unweighted(const char *filename) {
    FILE *f = fopen(filename, "r");
    if (!f) { printf("cannot open %s\n", filename); Graph e = {0,0,NULL}; return e; }
    int v; fscanf(f, "%d", &v);
    Graph g = create_graph(v);
    int from, to;
    while (fscanf(f, "%d,%d", &from, &to) == 2)
        add_edge_undirected(&g, from, to, 1);
    fclose(f);
    return g;
}

// generates a random connected graph for testing
Graph generate_random_graph(int v, int e, int max_weight) {
    Graph g = create_graph(v);
    // first make a chain so graph is connected
    for (int i = 0; i < v - 1; i++) {
        int w = 1 + rand() % max_weight;
        add_edge_undirected(&g, i, i + 1, w);
    }
    // add remaining random edges
    int added = v - 1;
    while (added < e) {
        int u = rand() % v;
        int t = rand() % v;
        if (u != t) {
            int w = 1 + rand() % max_weight;
            add_edge_undirected(&g, u, t, w);
            added++;
        }
    }
    return g;
}

void graph_test() {
    printf("=== Graph data structure tests ===\n\n");

    Graph g = create_graph(5);
    add_edge_undirected(&g, 0, 1, 4);
    add_edge_undirected(&g, 0, 2, 1);
    add_edge_undirected(&g, 1, 3, 1);
    add_edge_undirected(&g, 2, 1, 2);
    add_edge_undirected(&g, 2, 3, 5);
    add_edge_undirected(&g, 3, 4, 3);

    printf("Test graph (5 vertices, 6 undirected edges):\n");
    print_graph(&g);

    printf("\nDegrees:\n");
    for (int i = 0; i < g.V; i++)
        printf("  vertex %d: degree %d\n", i, out_degree(&g, i));

    printf("\nVertex count: %d, Edge count: %d\n", g.V, g.E);
    printf("Pass: %s\n", (g.V == 5 && g.E == 12) ? "YES" : "NO"); // 6 undirected = 12 directed

    destroy_graph(&g);
    printf("Graph tests complete.\n");
}
