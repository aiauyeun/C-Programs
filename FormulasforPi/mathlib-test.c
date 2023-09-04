#include "mathlib.h"

#include <math.h>
#include <stdio.h>
#include <unistd.h>
#define OPTIONS "aebmrvwnsh:"
extern int optind, opterr, optopt;

int main(int argc, char **argv) {
    int opt = 0;
    opterr = 0;
    int e_num = 0;
    int bbp = 0;
    int madh = 0;
    int e_pi = 0;
    int viet = 0;
    int wal = 0;
    int nwt = 0;
    int stat = 0;
    int hlp = 0;
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'a':
            e_num = 1;
            bbp = 1;
            madh = 1;
            e_pi = 1;
            viet = 1;
            wal = 1;
            nwt = 1;
            break;
        case 'e': e_num = 1; break;
        case 'b': bbp = 1; break;
        case 'm': madh = 1; break;
        case 'r': e_pi = 1; break;
        case 'v': viet = 1; break;
        case 'w': wal = 1; break;
        case 'n': nwt = 1; break;
        case 's': stat = 1; break;
        case 'h': hlp = 1; break;
        //error message
        case '?': hlp = 1; break;
        default: hlp = 1; break;
        }
    }
    if (hlp) {
        printf(""
               "SYNOPSIS\n     This is a test to observe some mathematical formulas and "
               "functions!\n OPTIONS \n     -a Runs all tests. \n     -e Runs e test \n     -b "
               "Runs BBP pi test. \n     -m Runs Madhava pi test. \n     -r Runs Euler pi "
               "test. \n     -v Runs Viete pi test. \n     -w Runs Wallis pi test. \n     -n "
               "Runs Newton Square Root Tests. \n     -s Print the Number of terms for each "
               "printed function \n     -h Displays this help screen.\n"
               "");
    } else {
        if (e_num) {
            printf("e() = %16.15lf, M_E = %16.15lf, diff = %16.15lf\n", e(), M_E, M_E - e());
            if (stat) {
                printf("e() terms = %d\n", e_terms());
            }
        }
        if (bbp) {
            printf("pi_bbp() = %16.15lf, M_PI = %16.15lf, diff = %16.15lf\n", pi_bbp(), M_PI,
                M_PI - pi_bbp());
            if (stat) {
                printf("pi_bbp() terms = %d\n", pi_bbp_terms());
            }
        }
        if (madh) {
            printf("pi_madhava() = %16.15lf, M_PI = %16.15lf, diff = %16.15lf\n", pi_madhava(),
                M_PI, M_PI - pi_madhava());
            if (stat) {
                printf("pi_madhava() terms = %d\n", pi_madhava_terms());
            }
        }
        if (e_pi) {
            printf("pi_euler() = %16.15lf, M_PI = %16.15lf, diff = %16.15lf\n", pi_euler(), M_PI,
                M_PI - pi_euler());
            if (stat) {
                printf("pi_euler() terms = %d\n", pi_euler_terms());
            }
        }
        if (viet) {
            printf("pi_viete() = %16.15lf, M_PI = %16.15lf, diff = %16.15lf\n", pi_viete(), M_PI,
                M_PI - pi_viete());
            if (stat) {
                printf("pi_viete() terms = %d\n", pi_viete_factors());
            }
        }
        if (wal) {
            printf("pi_wallis() = %16.15lf, M_PI = %16.15lf, diff = %16.15lf\n", pi_wallis(), M_PI,
                M_PI - pi_wallis());
            if (stat) {
                printf("pi_wallis() terms = %d\n", pi_wallis_factors());
            }
        }
        if (nwt) {
            for (double i = 0.0; i <= 9.95; i += 0.1) {
                printf("sqrt_newton(%.2lf) = %16.15lf, sqrt(%.2lf) = %16.15lf, diff = %16.15lf\n",
                    i, sqrt_newton(i), i, sqrt(i), sqrt(i) - sqrt_newton(i));
                if (stat) {
                    printf("sqrt_newton() terms = %d\n", sqrt_newton_iters());
                }
            }
        }
        if (e_num == 0 && bbp == 0 && madh == 0 && e_pi == 0 && viet == 0 && wal == 0 && nwt == 0) {
            hlp = 1;
        }
        if (hlp) {
            printf(""
                   "SYNOPSIS\n     This is a test to observe some mathematical formulas and "
                   "functions!\n OPTIONS \n     -a Runs all tests. \n     -e Runs e test \n     -b "
                   "Runs BBP pi test. \n     -m Runs Madhava pi test. \n     -r Runs Euler pi "
                   "test. \n     -v Runs Viete pi test. \n     -w Runs Wallis pi test. \n     -n "
                   "Runs Newton Square Root Tests. \n     -s Print the Number of terms for each "
                   "printed function \n     -h Displays this help screen.\n"
                   "");
        }
    }

    return 0;
}
