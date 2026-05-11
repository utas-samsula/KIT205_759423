#include "graph.h"

// loads graph - unweighted version for pagerank test input (no weights)
Graph load_graph_unweighted(char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("error opening file %s\n", filename);
        Graph empty = {0, NULL};
        return empty;
    }

    int v;
    fscanf(file, "%d", &v);
    Graph g = create_graph(v);

    int from, to;
    while (fscanf(file, "%d,%d", &from, &to) == 2) {
        add_edge(&g, from, to, 0);
    }

    fclose(file);
    return g;
}

int main() {
    printf("=== week 9 - pagerank ===\n\n");

    Graph g = load_graph_unweighted("pagerank_test.txt");

    printf("graph has %d vertices\n\n", g.V);

    print_in_degrees(&g);
    printf("\n");

    // run pagerank for 100 iterations
    pagerank(&g, 100);

    destroy_graph(&g);
    return 0;
}