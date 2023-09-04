#include "mathlib.h"

#include <stdio.h>

static double k = 1;

double pi_viete() {
    k = 1;
    double a = sqrt_newton(2);
    double term = a / 2.0;
    double total = term;
    do {
        a = sqrt_newton(2 + a);
        term = a / 2.0;
        total *= term;
        k++;
    } while ((1 - absolute(term)) > EPSILON);
    return 2.0 / total;
}

int pi_viete_factors() {
    return k;
}
