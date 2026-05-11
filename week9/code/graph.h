#ifndef GRAPH_H
#define GRAPH_H

#include <stdio.h>
#include <stdlib.h>

typedef struct edge {
    int to_vertex;
    int weight;
} Edge;

typedef struct edgeNode {
    Edge edge;
    struct edgeNode *next;
} *EdgeNodePtr;

typedef struct edgeList {
    EdgeNodePtr head;
} EdgeList;

typedef struct graph {
    int V;
    EdgeList *edges;
} Graph;

Graph create_graph(int v);
void add_edge(Graph *self, int from, int to, int w);
void print_graph(Graph *self);
void print_in_degrees(Graph *self);
void destroy_graph(Graph *self);
void pagerank(Graph *self, int iterations);

#endif