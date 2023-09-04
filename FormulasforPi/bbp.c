#include "mathlib.h"

#include <stdio.h>

static int k = 0;

double pi_bbp() {
    k = 0;
    double in = (3.3 - (1.0 / 6));
    double out = 1.0;
    double exp = 1.0;
    double term_val = in * out;
    double total = term_val;
    k++;
    do {
        exp = 16 * exp;
        out = 1 / exp;
        in = ((4.0 / ((8.0 * k) + 1.0)) - (2.0 / ((8.0 * k) + 4.0)) - (1.0 / ((8.0 * k) + 5.0))
              - (1.0 / ((8.0 * k) + 6.0)));
        term_val = in * out;
        total += term_val;
        k++;
    } while (term_val > EPSILON);
    return total;
}

int pi_bbp_terms() {
    return k;
}
