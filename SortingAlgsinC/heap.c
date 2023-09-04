#include "heap.h"

#include "stats.h"

#include <stdbool.h>
#include <stdio.h>

uint32_t max_child(Stats *stats, uint32_t *arr, int first, int last) {
    //int left = move(stats,2 * first); 635
    //int right = move(stats,left + 1); 635
    int left = 2 * first;
    int right = left + 1;
    if (right <= last && cmp(stats, arr[right - 1], arr[left - 1]) == 1) {
        return right;
    }
    return left;
}

void fix_heap(Stats *stats, uint32_t *arr, int first, int last) {
    int found = false;
    int mother = first;
    int great = max_child(stats, arr, mother, last);
    while (mother <= last / 2 && (found == false)) {
        if (cmp(stats, arr[mother - 1], arr[great - 1]) == -1) {
            swap(stats, &arr[mother - 1], &arr[great - 1]);
            mother = great; //486
            great = max_child(stats, arr, mother, last); //486
        } else {
            found = true;
        }
    }
}

void build_heap(Stats *stats, uint32_t *arr, int first, int last) {
    for (int i = last / 2; i > (first - 1); i--) {
        fix_heap(stats, arr, i, last);
    }
}

void heap_sort(Stats *stats, uint32_t *arr, uint32_t last) {
    int first = 1;
    build_heap(stats, arr, first, last);
    for (int i = last; i > first; i--) {
        swap(stats, &arr[first - 1], &arr[i - 1]);
        fix_heap(stats, arr, first, i - 1);
    }
}