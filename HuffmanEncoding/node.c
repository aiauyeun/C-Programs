#include "node.h"

#include <stdio.h>
#include <stdlib.h>

//Creates a node and set its symbol and weight fields. Returns a pointer to the new node
Node *node_create(uint8_t symbol, double weight) {
    Node *n = calloc(1, sizeof(Node));
    n->symbol = symbol;
    n->weight = weight;
    return n;
}

//frees the node!
void node_free(Node **node) {
    // if((*node)->left != NULL) { do we want this code?
    //     node_free((*node)->left);
    // }
    // if((*node)->right != NULL) {
    //     node_free((*node)->right);
    //}
    free(*node);
    *node = NULL;
}

//prints the node tree!!!
void node_print_tree(Node *tree, char ch, int indentation) {
    if (tree == NULL)
        return;
    node_print_tree(tree->right, '/', indentation + 3);
    printf("%*cweight = %.0f", indentation + 1, ch, tree->weight);
    if (tree->left == NULL && tree->right == NULL) {
        if (' ' <= tree->symbol && tree->symbol <= '~') {
            printf(", symbol = '%c'", tree->symbol);
        } else {
            printf(", symbol = 0x%02x", tree->symbol);
        }
    }
    printf("\n");
    node_print_tree(tree->left, '\\', indentation + 3);
}
