#include "graph.h"

Graph create_graph(int v) {
    Graph g;
    g.V = v;
    g.edges = malloc(v * sizeof(EdgeList));
    for (int i = 0; i < v; i++) {
        g.edges[i].head = NULL;
    }
    return g;
}

void add_edge(Graph *self, int from, int to, int w) {
    EdgeNodePtr new_node = malloc(sizeof *new_node);
    new_node->edge.to_vertex = to;
    new_node->edge.weight = w;
    new_node->next = self->edges[from].head;
    self->edges[from].head = new_node;
}

void print_graph(Graph *self) {
    for (int i = 0; i < self->V; i++) {
        printf("%d -> ", i);
        EdgeNodePtr current = self->edges[i].head;
        while (current != NULL) {
            printf("(%d) ", current->edge.to_vertex);
            current = current->next;
        }
        printf("\n");
    }
}

void print_in_degrees(Graph *self) {
    int *in_degrees = malloc(self->V * sizeof(int));
    for (int i = 0; i < self->V; i++) in_degrees[i] = 0;

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

// pagerank - unnormalised version as per tutorial pseudocode
// equation: PR(v) = d * sum(PR(u)/out_degree(u)) + (1-d)
void pagerank(Graph *self, int iterations) {
    int v = self->V;
    double d = 0.85; // standard damping factor

    // calculate out-degrees first
    double *out_degrees = malloc(v * sizeof(double));
    for (int i = 0; i < v; i++) {
        out_degrees[i] = 0;
        EdgeNodePtr current = self->edges[i].head;
        while (current != NULL) {
            out_degrees[i]++;
            current = current->next;
        }
    }

    // initialise all pageranks to 1
    double *pr = malloc(v * sizeof(double));
    for (int i = 0; i < v; i++) {
        pr[i] = 1.0;
    }

    double *sums = malloc(v * sizeof(double));

    for (int iter = 0; iter < iterations; iter++) {

        // step 1 - reset all sums to zero
        for (int i = 0; i < v; i++) {
            sums[i] = 0.0;
        }

        // step 2 - loop through all edges, add PR(from)/out(from) to sum[to]
        // these loops MUST stay separate as per tutorial instructions
        for (int i = 0; i < v; i++) {
            if (out_degrees[i] > 0) {
                EdgeNodePtr current = self->edges[i].head;
                while (current != NULL) {
                    int to = current->edge.to_vertex;
                    sums[to] += pr[i] / out_degrees[i];
                    current = current->next;
                }
            }
        }

        // step 3 - calculate new pageranks using damping factor
        for (int i = 0; i < v; i++) {
            pr[i] = (1.0 - d) + d * sums[i];
        }
    }

    printf("pagerank results after %d iterations:\n", iterations);
    for (int i = 0; i < v; i++) {
        printf("  vertex %d: %.4f\n", i, pr[i]);
    }

    free(pr);
    free(sums);
    free(out_degrees);
}