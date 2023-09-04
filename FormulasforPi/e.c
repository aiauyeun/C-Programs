#include "mathlib.h"

#include <math.h>
#include <stdio.h>

static int k = 0;

double e() {
    k = 0;
    double total = 0;
    double term_val = 1;
    float divisor = 1;
    do {
        if (k == 0) {
            total += term_val;
            k++;
        } else {
            divisor = divisor * k;
            term_val = 1.0 / divisor;
            total += term_val;
            k++;
        }
    } while (term_val > EPSILON);
    return total;
}

int e_terms(void) {
    return k - 1;
}

// int main(void) {
// 	double ee = e();
// 	printf("e() = %16.15lf, M_E = %16.15lf, diff = %16.15lf\n", ee, M_E, M_E - ee);

// 	printf("\n%d", e_terms());
// }
