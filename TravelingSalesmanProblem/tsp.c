#include "graph.h"
#include "path.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define OPTIONS "i:o:dh"
#include <linux/limits.h>

extern int optind, opterr, optopt;
void dfs(int node, Path *check, Path *shortest, Graph *map);

void dfs(int node, Path *check, Path *shortest, Graph *map) {
    //add node to path
    path_add(check, node, map);
    //mark node as visited
    graph_visit_vertex(map, node);

    //for every verticie in graph
    for (uint32_t i = 0; i < graph_vertices(map); i++) {
        //If there is a nonzero edge in node that hasn't been visited, run dfs again on that edge as the node.
        if ((graph_get_weight(map, node, i) != 0) && (graph_visited(map, i) == false)) {
            dfs(i, check, shortest, map);
        }

        if ((path_vertices(check)) == graph_vertices(map)) {
            //path_print(check, stdout, map);
            if (graph_get_weight(map, node, 0) != 0) {
                path_add(check, 0, map);

                if ((path_distance(shortest) == 0)
                    || (path_distance(shortest) > path_distance(check))) {
                    //path_print(check,stdout,map);
                    path_copy(shortest, check);
                    //path_print(shortest,stdout,map);
                }
                path_remove(check, map);
            }
        }
    }
    graph_unvisit_vertex(map, node);
    path_remove(check, map);
    //If the path's verticies are one less than the total amount of verticies: check if the current node has an edge that
    //goes to back to the start. If so, add that edge to the path and then check if this path is shorter than shortest.

    //printf("\n path \n");
    //path_print(check,stdout,map);

    //Unvisit the node and remove the node from path to check other possible paths with dfs.
}

int main(int argc, char **argv) {
    int opt = 0;
    opterr = 0;
    char *help = "test";
    FILE *infile = stdin;
    FILE *outfile = stdout;
    bool directed = false;
    Path *check = path_create(100);
    Path *shortest = path_create(100);
    //int justd = 1;
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'i':
            //justd = 0;
            infile = fopen(optarg, "r");
            help = optarg;
            if (infile == NULL || (argc < 3)) {
                fprintf(stderr, "tsp:  error reading input file %s\n", optarg);
                return 0;
            }
            break;
        case 'o':
            //justd = 0;
            outfile = fopen(optarg, "w");
            if (outfile == NULL || argc < 3) {
                printf("tsp:  error reading output file %s\n", optarg);
                return 0;
            }
            break;
        case 'd': directed = true; break;
        case 'h':
            //justd = 0;
            printf("Usage: tsp [options]\n\n"
                   "-i infile    Specify the input file path containing the cities and edges\n"
                   "             of a graph. If not specified, the default input should be\n"
                   "             set as stdin.\n\n"
                   "-o outfile   Specify the output file path to print to. If not specified,\n"
                   "             the default output should be set as stdout.\n\n"
                   "-d           Specifies the graph to be directed.\n\n"
                   "-h           Prints out a help message describing the purpose of the\n"
                   "             graph and the command-line options it accepts, exiting the\n"
                   "             program afterwards.\n");
            return 0;
        }
    }
    char line[PATH_MAX];
    //get # of verticies
    if (fgets(line, sizeof(line), infile) == NULL) {
        fprintf(stderr, "tsp:  error reading input file %s\n", help);
        return 0;
    }
    int vertices = atoi(line);
    Graph *graph = graph_create(vertices, directed); //creating graph

    //**FILLING GRAPH**

    //add vertices.
    for (int i = 0; i < vertices; i++) {
        fgets(line, sizeof(line), infile);
        //fscanf /n
        char *name = line;
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }
        graph_add_vertex(graph, name, i);
        //printf("Line: %s\n", name);
    }

    //find # of edges
    fgets(line, sizeof(line), infile);
    int edges = atoi(line);
    //printf("edges: %d\n",edges);

    int strt, end, wght;
    //adds edges to graph
    for (int i = 0; i < edges; i++) {
        //using & here to get the mem address to the vars so fscanf
        //can put the scanned variable into specified variable.
        fscanf(infile, "%d %d %d", &strt, &end, &wght);
        //printf("%d\n", wght);
        graph_add_edge(graph, strt, end, wght);
    }

    //**GRAPH FILLED**

    //graph_print(graph);

    //printf("==== PATH TESTING ====");

    dfs(0, check, shortest, graph);
    if (path_distance(shortest) != 0) {
        //printf("\nshortest output:\n");
        path_print(shortest, outfile, graph);
    } else {
        fprintf(outfile, "No path found! Alissa is lost!\n");
    }
    path_free(&check);
    path_free(&shortest);
    graph_free(&graph);
    fclose(infile);
    fclose(outfile);

    return 0;
}
