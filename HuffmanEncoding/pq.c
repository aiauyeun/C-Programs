#include "pq.h"

#include <stdio.h>
#include <stdlib.h>

/* put in pq.c */
typedef struct ListElement ListElement;
struct ListElement {
    Node *tree;
    ListElement *next;
};
struct PriorityQueue {
    ListElement *list;
};

//creates a priority queue!
PriorityQueue *pq_create(void) {
    PriorityQueue *p = calloc(1, sizeof(PriorityQueue));
    return p;
}

void pq_free(PriorityQueue **q) {
    free(*q);
    *q = NULL;
}

bool pq_is_empty(PriorityQueue *q) {
    if (q->list == NULL) {
        return true;
    }
    return false;
}

//checks if the q->list == 0
bool pq_size_is_1(PriorityQueue *q) {
    if (pq_is_empty(q)) {
        return false;
    }
    if (q->list->next == NULL) {
        return true;
    }
    return false;
}

//If the queue is empty, return false. Otherwise, remove the queue
//element with the lowest weight, set e to
//point to it, set parameter *tree = e->tree, call free(e), and return true.

bool dequeue(PriorityQueue *q, Node **tree) {
    if (pq_is_empty(q)) {
        return false;
    }
    if (pq_size_is_1(q)) {
        ListElement *e = q->list;
        *tree = e->tree;
        free(e);
        q->list = NULL;
        return true;
    }
    ListElement *e = q->list;

    q->list = q->list->next;
    *tree = e->tree;
    free(e);
    return true;
    // ListElement *cur = q->list;
    // if(cur->next != NULL) {
    //     ListElement *next = cur->next;
    //     while(next->next != NULL) {
    //         cur = cur->next;
    //         next = next->next;
    //     }
    // }

    // ListElement *e = cur->next;
    // cur->next = NULL;
    // free(e);
    // return true;
}

void pq_print(PriorityQueue *q) {
    assert(q != NULL);
    ListElement *e = q->list;
    int position = 1;
    while (e != NULL) {
        if (position++ == 1) {
            printf("=============================================\n");
        } else {
            printf("---------------------------------------------\n");
        }
        node_print_tree(e->tree, '<', 2);
        e = e->next;
    }
    printf("=============================================\n");
}

//compare ascii if they the same weight.
bool pq_less_than(Node *n1, Node *n2) {
    if (n1->weight < n2->weight)
        return true;
    if (n1->weight > n2->weight)
        return false;
    return n1->symbol < n2->symbol;
}

//priority queue is like a tree with a next next next. When we enqueue, we find its ordered place based on it's weight!
void enqueue(PriorityQueue *q, Node *tree) {
    //create new list element e.
    ListElement *e = calloc(1, sizeof(ListElement));
    e->tree = tree;
    //if pq empty make q's list e
    if (pq_is_empty(q)) {
        q->list = e;
        return;
    }
    //if tree's weight is smaller than q's smallest, make e the head.
    if (pq_less_than(tree, q->list->tree)) {
        e->next = q->list;
        q->list = e;
        return;
    }
    //braindead implementation...
    ListElement *cur = q->list;
    //Check if another node exists after head if so, store that ListEl in next
    //then check if we should insert e there.
    if (cur->next != NULL) {
        ListElement *next = cur->next;
        if (pq_less_than(tree, next->tree)) {
            e->next = next;
            cur->next = e;
            return;
        }
        //iterate thru to the rest of the pq to see where we should insert.
        while (next->next != NULL) {
            next = next->next;
            cur = cur->next;
            if (pq_less_than(tree, next->tree)) {
                e->next = next;
                cur->next = e;
                return;
            }
        }
        //if it is not smaller than any of the #s, put it at the end
        next->next = e;
    } else {
        //else condition: if next don't exist, then we just put e after cur!
        cur->next = e;
    }
    //traver5e queue chain

    //if it i5 the large5t num, 5et it to be the old la5t one'5 next.
    //if((next->tree->weight))
}
