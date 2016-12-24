/*
 * LibHLL demo application.
 *
 * Usage:
 *
 * cat /usr/share/dict/words | ./hll_demo $(wc -l < /usr/share/dict/words)
 *
 */

#define _GNU_SOURCE
#include <hll/hll.h>
#include <hll/hll_private.h>
#include <city/city.h>

#include <stdio.h>
#include <stdlib.h>

static const int N_ESTIMATORS = 16-4+1; // one for each number of buckets

int main(int argc, const char *argv[])
{
    size_t exact_number = 0;

    if (argc > 1) {
        exact_number = strtoul(argv[1], 0, 10);
    }

    hll_t *hll[N_ESTIMATORS];
    
    for (int i = 0; i < N_ESTIMATORS; i++) {
        hll[i] = hll_create(i+4);
    }
   
    char *line = NULL;
    size_t buflen = 0;
    size_t linelen = 0;
    while ((linelen = getline(&line, &buflen, stdin)) != -1) {
        for (int i = 0; i < N_ESTIMATORS; i++) {
            hll_add(hll[i], line, linelen-1);
        }
    }

    int best_n_buckets = 0;
    double min_error = 101.0;
    for (int i = 0; i < N_ESTIMATORS; i++) {
        double error = 0.0;
        size_t estimate = hll_get_estimate(hll[i]);

        if (exact_number > 0) {
            error = abs(exact_number - estimate) / (double)exact_number * 100.0;
            if (error < min_error) {
                best_n_buckets = i+4;
                min_error = error;
            }
        }
        
        printf("estimate with %2d buckets: %6lu, error: %5.2f%%\n", i+4, estimate, error);
        hll_release(hll[i]);
    }

    if (best_n_buckets > 0) {
        printf("minimal error achieved with %d buckets, as %.2f%% of the exact value %lu\n", 
               best_n_buckets, min_error, exact_number);
    }

    free(line);

    return 0;
}
