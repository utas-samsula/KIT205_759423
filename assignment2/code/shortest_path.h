#ifndef SHORTEST_PATH_H
#define SHORTEST_PATH_H

#include "graph.h"

// stores result of shortest path computation
typedef struct pathResult {
    int *dist;      // distance from source to each vertex
    int *prev;      // predecessor for path reconstruction
    int V;          // number of vertices
    long time_ms;   // how long the algorithm took
} PathResult;

PathResult dijkstra(Graph *g, int source);
PathResult bellman_ford(Graph *g, int source);
void print_path(PathResult *r, int source, int dest);
void print_all_distances(PathResult *r, int source);
void destroy_result(PathResult *r);
void shortest_path_test();

#endif
