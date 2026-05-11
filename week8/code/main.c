#include "graph.h"

// loads graph from file - format is V on first line then from,to,weight per line
Graph load_graph(char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("error opening file %s\n", filename);
        Graph empty = {0, NULL};
        return empty;
    }

    int v;
    fscanf(file, "%d", &v);
    Graph g = create_graph(v);

    int from, to, weight;
    while (fscanf(file, "%d,%d,%d", &from, &to, &weight) == 3) {
        add_edge(&g, from, to, weight);
    }

    fclose(file);
    return g;
}

int main() {
    printf("=== week 8 - graph from file ===\n\n");

    Graph g = load_graph("graph.txt");

    printf("adjacency list:\n");
    print_graph(&g);

    printf("\n");
    print_in_degrees(&g);

    destroy_graph(&g);
    return 0;
}