#include "mathlib.h"

#include <stdio.h>

static int k = 0;

double pi_madhava() {
    k = 0;
    double twelve = sqrt_newton(12);
    double total = 0;
    double cur_term = 1.0;
    double top = 0.0;
    double bottom = 1.0;
    long ex = 1.0;
    do {
        if (k == 0) {
            total += 1;
            k++;
        } else {
            ex = ex * -3;
            top = (1.0 / ex);
            bottom = ((2.0 * k) + 1.0);
            cur_term = top / bottom;
            total += cur_term;
            k++;
        }
    } while (absolute(cur_term) > EPSILON);
    return total * twelve;
}

int pi_madhava_terms() {
    return k;
}
