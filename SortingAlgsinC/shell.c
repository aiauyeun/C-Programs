#include "shell.h"

#include "gaps.h"
#include "stats.h"

#include <stdio.h>

void shell_sort(Stats *stats, uint32_t *A, uint32_t length) {
    int len = sizeof(gaps) / sizeof(gaps[0]);
    for (int j = 0; j < len; j++) {
        for (int i = gaps[j]; i < length; i++) {
            int k = i;
            int temp = move(stats, A[i]);
            while (k >= gaps[j] && (cmp(stats, temp, A[k - gaps[j]]) == -1)) {
                A[k] = move(stats, A[k - gaps[j]]);
                k -= gaps[j];
            }
            A[k] = move(stats, temp);
        }
    }
}
