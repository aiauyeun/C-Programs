#include "mathlib.h"

#include <stdio.h>

static int iters = 0;

double sqrt_newton(double x) {
    iters = 0;
    double next_y = 1.0;
    double y = 0.0;
    while (absolute(next_y - y) > EPSILON) {
        iters++;
        y = next_y;
        next_y = .5 * (y + x / y);
    }
    return y;
}

int sqrt_newton_iters() {
    return iters;
}
