#include "mathlib.h"

#include <stdio.h>

static long k = 1;

double pi_euler() {
    k = 1;
    double total = 0;
    double term_val = 1;
    double bottom = 1;
    do {
        bottom = k * k;
        term_val = 1.0 / bottom;
        total += term_val;
        k++;
    } while (term_val > EPSILON);
    total = total * 6;
    return sqrt_newton(total);
}

int pi_euler_terms() {
    return k - 1;
}
