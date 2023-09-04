#include "graph.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct graph {
    uint32_t vertices;
    bool directed;
    bool *visited;
    char **names;
    uint16_t **weights;
} Graph;

Graph *graph_create(uint32_t vertices, bool directed) {
    Graph *g = calloc(1, sizeof(Graph));
    g->vertices = vertices;
    g->directed = directed;
    // use calloc to initialize everything with zeroes
    g->visited = calloc(vertices, sizeof(bool));
    g->names = calloc(vertices, sizeof(char *));
    // allocate g->weights with a pointer for each row
    g->weights = calloc(vertices, sizeof(g->weights[0]));
    // allocate each row in the adjacency matrix
    for (uint32_t i = 0; i < vertices; ++i) {
        g->weights[i] = calloc(vertices, sizeof(g->weights[0][0]));
    }
    return g;
}

void graph_free(Graph **gp) {
    if ((*gp)->names != NULL && gp != NULL) {
        for (uint32_t i = 0; i < (*gp)->vertices; ++i) {
            free((*gp)->weights[i]);
            free((*gp)->names[i]);
        }
    }
    free((*gp)->names);
    free((*gp)->weights);
    free((*gp)->visited);
    free(*gp);
    *gp = NULL;
}

uint32_t graph_vertices(const Graph *g) {
    return (g)->vertices;
}

//adds a weight to the adjacency matrix for weight[start][end]
void graph_add_edge(Graph *g, uint32_t start, uint32_t end, uint32_t weight) {
    if (!(g->directed)) {
        g->weights[start][end] = weight;
        g->weights[end][start] = weight;
    } else {
        g->weights[start][end] = weight;
    }
}

uint32_t graph_get_weight(const Graph *g, uint32_t start, uint32_t end) {
    return g->weights[start][end];
}

void graph_visit_vertex(Graph *g, uint32_t v) {
    g->visited[v] = true;
}

void graph_unvisit_vertex(Graph *g, uint32_t v) {
    g->visited[v] = false;
}

bool graph_visited(const Graph *g, uint32_t v) {
    if (g->visited[v] == true) {
        return true;
    }
    return false;
}

char **graph_get_names(const Graph *g) {
    return g->names;
}

void graph_add_vertex(Graph *g, const char *name, uint32_t v) {
    if (g->names[v])
        free(g->names[v]);
    g->names[v] = strdup(name);
}

const char *graph_get_vertex_name(const Graph *g, uint32_t v) {
    return g->names[v];
}

void graph_print(const Graph *g) {
    printf("Weights");
    for (uint32_t i = 0; i < g->vertices; i++) {
        printf("\n");
        for (uint32_t j = 0; j < g->vertices; ++j) {
            printf("%d ", g->weights[i][j]);
        }
    }
    printf("\n\nVertices: %u\n", graph_vertices(g));
    printf("\nVisited:\n");
    for (uint32_t i = 0; i < g->vertices; i++) {
        printf("%d", g->visited[i]);
    }
    printf("\nNames:\n");
    for (uint32_t i = 0; i < g->vertices; i++) {
        printf("%s", g->names[i]);
    }
    printf("\nDirected? %d\n", g->directed);
}
