#include "batcher.h"
#include "gaps.h"
#include "heap.h"
#include "insert.h"
#include "quick.h"
#include "set.h"
#include "shell.h"
#include "stats.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define OPTIONS "aishqbr:n:p:H"

uint32_t *genArray(int seed, int size) {
    srandom(seed);
    uint32_t *arr = calloc(size, sizeof(uint32_t));
    for (int i = 0; i < size; i++) {
        int num = random() & 0b00111111111111111111111111111111;
        arr[i] = num;
    }
    return arr;
}

void printArray(uint32_t *arr, int ele, int size) {
    if (ele > size) {
        ele = size;
    }
    for (int i = 0; i < ele; i++) {
        if (i != 0 && i % 5 == 0) {
            printf("\n");
        }
        printf("%13d", arr[i]);
    }
    if (ele > 0) {
        printf("\n");
    }
}

int main(int argc, char **argv) {
    Stats stat;
    int seed = 13371453;
    int opt = 0;
    Set set = set_empty();
    int ele = 100;
    int size = 100;
    if (argc == 1) {
        printf(
            "Select at least one sort to perform.\nSYNOPSIS\n   A collection of comparison-based "
            "sorting algorithms.\n\nUSAGE\n   ./sorting-x86 [-Hahbsqi] [-n length] [-p elements] "
            "[-r seed]\nOPTIONS\n-H              Display program help and usage.\n-a              "
            "Enable all sorts.\n-h              Enable Heap Sort.\n-b              Enable Batcher "
            "Sort.\n-s              Enable Shell Sort.\n-q              Enable Quick Sort.\n-i     "
            "         Enable Insertion Sort.\n-n length       Specify number of array elements "
            "(default: 100).\n-p elements     Specify number of elements to print (default: "
            "100).\n-r seed         Specify random seed (default: 13371453).\n");
    }
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'a':
            set = set_insert(set, 0);
            set = set_insert(set, 1);
            set = set_insert(set, 2);
            set = set_insert(set, 3);
            set = set_insert(set, 4);
            break;
        case 'b': set = set_insert(set, 4); break;
        case 'i': set = set_insert(set, 0); break;
        case 's': set = set_insert(set, 2); break;
        case 'h': set = set_insert(set, 1); break;
        case 'q': set = set_insert(set, 3); break;
        case 'r': seed = atoi(optarg); break;
        case 'n': size = atoi(optarg); break;
        case 'p': ele = atoi(optarg); break;
        case 'H': set = set_insert(set, 7); break;
        case '?': set = set_insert(set, 7); break;
        default:
            fprintf(stderr, "test");
            set = set_insert(set, 7);
            break;
        }
    }
    uint32_t *arr;
    if (set_member(set, 7)) {
        printf(
            "Select at least one sort to perform.\nSYNOPSIS\n   A collection of comparison-based "
            "sorting algorithms.\n\nUSAGE\n   ./sorting-x86 [-Hahbsqi] [-n length] [-p elements] "
            "[-r seed]\nOPTIONS\n-H              Display program help and usage.\n-a              "
            "Enable all sorts.\n-h              Enable Heap Sort.\n-b              Enable Batcher "
            "Sort.\n-s              Enable Shell Sort.\n-q              Enable Quick Sort.\n-i     "
            "         Enable Insertion Sort.\n-n length       Specify number of array elements "
            "(default: 100).\n-p elements     Specify number of elements to print (default: "
            "100).\n-r seed         Specify random seed (default: 13371453).\n");
    } else {
        for (int i = 0; i < 7; i++) {
            if (set_member(set, i)) {
                switch (i) {
                case 0:
                    arr = genArray(seed, size);
                    reset(&stat);
                    insertion_sort(&stat, arr, size);
                    print_stats(&stat, "Insertion Sort", size);
                    printArray(arr, ele, size);
                    free(arr);
                    break;
                case 2:
                    arr = genArray(seed, size);
                    reset(&stat);
                    shell_sort(&stat, arr, size);
                    print_stats(&stat, "Shell Sort", size);
                    printArray(arr, ele, size);
                    free(arr);
                    break;
                case 1:
                    arr = genArray(seed, size);
                    reset(&stat);
                    heap_sort(&stat, arr, size);
                    print_stats(&stat, "Heap Sort", size);
                    printArray(arr, ele, size);
                    free(arr);
                    break;
                case 3:
                    arr = genArray(seed, size);
                    reset(&stat);
                    quick_sort(&stat, arr, size);
                    print_stats(&stat, "Quick Sort", size);
                    printArray(arr, ele, size);
                    free(arr);
                    break;
                case 4:
                    arr = genArray(seed, size);
                    reset(&stat);
                    batcher_sort(&stat, arr, size);
                    print_stats(&stat, "Batcher Sort", size);
                    printArray(arr, ele, size);
                    free(arr);
                    break;
                }
            }
        }
    }
    return 0;
}