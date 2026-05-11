#include "graph.h"

// sets up empty graph with v vertices
Graph create_graph(int v) {
    Graph g;
    g.V = v;
    g.edges = malloc(v * sizeof(EdgeList));
    for (int i = 0; i < v; i++) {
        g.edges[i].head = NULL;
    }
    return g;
}

// insert at front of the edge list for vertex 'from'
void add_edge(Graph *self, int from, int to, int w) {
    EdgeNodePtr new_node = malloc(sizeof *new_node);
    new_node->edge.to_vertex = to;
    new_node->edge.weight = w;
    new_node->next = self->edges[from].head;
    self->edges[from].head = new_node;
}

// prints adjacency list for each vertex
void print_graph(Graph *self) {
    for (int i = 0; i < self->V; i++) {
        printf("%d -> ", i);
        EdgeNodePtr current = self->edges[i].head;
        while (current != NULL) {
            printf("(%d, w=%d) ", current->edge.to_vertex, current->edge.weight);
            current = current->next;
        }
        printf("\n");
    }
}

// calculates in-degree for each vertex by counting how many edges point to it
void print_in_degrees(Graph *self) {
    int *in_degrees = malloc(self->V * sizeof(int));
    for (int i = 0; i < self->V; i++) {
        in_degrees[i] = 0;
    }

    for (int i = 0; i < self->V; i++) {
        EdgeNodePtr current = self->edges[i].head;
        while (current != NULL) {
            in_degrees[current->edge.to_vertex]++;
            current = current->next;
        }
    }

    printf("in-degrees:\n");
    for (int i = 0; i < self->V; i++) {
        printf("  vertex %d: %d\n", i, in_degrees[i]);
    }
    free(in_degrees);
}

void destroy_graph(Graph *self) {
    for (int i = 0; i < self->V; i++) {
        EdgeNodePtr current = self->edges[i].head;
        while (current != NULL) {
            EdgeNodePtr to_free = current;
            current = current->next;
            free(to_free);
        }
    }
    free(self->edges);
    self->V = 0;
}