#include "graph.h"
#include "stack.h"

#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
typedef struct path {
    uint32_t total_weight;
    Stack *vertices;
} Path;

Path *path_create(uint32_t capacity) {
    Path *p = (Path *) malloc(sizeof(Path));
    p->total_weight = 0;
    p->vertices = stack_create(capacity);
    return p;
}

void path_free(Path **pp) {
    stack_free(&(*pp)->vertices);
    free(*pp);
    *pp = NULL;
}

uint32_t path_vertices(const Path *p) {
    return stack_size(p->vertices);
}

uint32_t path_distance(const Path *p) {
    return p->total_weight;
}

void path_add(Path *p, uint32_t val, const Graph *g) {
    uint32_t start = 0;
    stack_peek(p->vertices, &start);
    stack_push(p->vertices, val);
    p->total_weight += graph_get_weight(g, start, val);
}

uint32_t path_remove(Path *p, const Graph *g) {
    uint32_t now = 0;
    stack_pop(p->vertices, &now);
    if (stack_empty(p->vertices)) {
        //assert error
        p->total_weight -= graph_get_weight(g, 0, now);
    } else {
        uint32_t prev = 0;
        stack_peek(p->vertices, &prev);
        p->total_weight -= graph_get_weight(g, prev, now);
    }
    return now;
}

void path_copy(Path *dst, const Path *src) {
    dst->total_weight = src->total_weight;
    stack_copy(dst->vertices, src->vertices);
}

void path_print(const Path *p, FILE *f, const Graph *g) {
    fprintf(f, "Alissa starts at:\n");

    stack_print(p->vertices, f, graph_get_names(g));
    int dist = path_distance(p);
    fprintf(f, "Total Distance: %d\n", dist);
}
