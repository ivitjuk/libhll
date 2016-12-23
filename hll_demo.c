#define _GNU_SOURCE
#include <hll/hll.h>
#include <hll/hll_private.h>
#include <city/city.h>

#include <stdio.h>
#include <stdlib.h>

int main(int argc, const char *argv[])
{
    char *line = NULL;
    size_t len = 0;

    hll_init_lib();

    hll_t *hll[16-4+1];
    
    for (int i = 0; i < 16-4+1; i++) {
        hll[i] = hll_create(i+4);
    }
    
    while (getline(&line, &len, stdin) != -1) {
        for (int i = 0; i < 16-4+1; i++) {
            hll_add(hll[i], line, len);
        }
    }

    for (int i = 0; i < 16-4+1; i++) {
        printf("estimate %d: %lu\n", i+4, hll_get_estimate(hll[i]));
        hll_release(hll[i]);
    }

    free(line);

    return 0;
}
