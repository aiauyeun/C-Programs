#include "mathlib.h"

#include <stdio.h>

static long k = 1;

double pi_wallis() {
    k = 1;
    double top = 4;
    double bot = 3;
    double term = top / bot;
    double total = term;
    k++;
    do {
        top = 4.0 * (k * k);
        bot = top - 1.0;
        term = top / bot;
        total *= term;
        k++;
    } while ((absolute(1.0 - term)) > EPSILON);
    return 2.0 * total;
}

int pi_wallis_factors() {
    return k - 1;
}
