#include "quick.h"

#include "stats.h"

#include <stdio.h>

uint32_t partition(Stats *stats, uint32_t *arr, int lo, int hi) {
    int i = lo - 1;
    for (int j = lo; j < hi; j++) {
        //640c
        if (cmp(stats, arr[j - 1], arr[hi - 1]) == -1) {
            i += 1;
            swap(stats, &arr[i - 1], &arr[j - 1]); //855
            // int temp = arr[i-1];
            // arr[i-1] = arr[j-1];
            // arr[j-1] = temp;
        }
    }
    swap(stats, &arr[i], &arr[hi - 1]);
    // int temp = arr[i];
    // arr[i] = arr[hi-1];
    // arr[hi-1] = temp;
    return i + 1;
}

void quick_sorter(Stats *stats, uint32_t *arr, int lo, int hi) {
    if (lo < hi) {
        uint32_t p = partition(stats, arr, lo, hi);
        quick_sorter(stats, arr, lo, p - 1);
        quick_sorter(stats, arr, p + 1, hi);
    }
}

void quick_sort(Stats *stats, uint32_t *arr, uint32_t n) {
    quick_sorter(stats, arr, 1, n);
}