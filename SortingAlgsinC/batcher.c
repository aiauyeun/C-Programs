#include "batcher.h"

#include "stats.h"

#include <stdio.h>

void comparator(Stats *stats, uint32_t *arr, int x, int y) {
    if (cmp(stats, arr[x], arr[y]) == 1) {
        swap(stats, &arr[x], &arr[y]);
    }
}

void batcher_sort(Stats *stats, uint32_t *arr, uint32_t size) {
    if (size == 0) {
        return;
    }
    int bits = 0;
    int n = size;
    while (n > 0) {
        bits++;
        n >>= 1;
    }
    uint32_t p = 1 << (bits - 1);

    while (p > 0) {
        uint32_t q = 1 << (bits - 1);
        int r = 0;
        uint32_t d = p;
        while (d > 0) {
            for (int i = 0; i < (size - d); i++) {
                if ((i & p) == r) {
                    comparator(stats, arr, i, i + d);
                }
            }
            d = q - p;
            q >>= 1;
            r = p;
        }
        p >>= 1;
    }
}