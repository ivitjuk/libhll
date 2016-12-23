#include <hll/hll.h>
#include <hll/hll_private.h>
#include <city/city.h>

#include <stdio.h>
#include <stdint.h>

int main(int argc, const char *argv[])
{
    int ret = 0;

    hll_init_lib();
    hll_t *hll = hll_create(4);
    
    uint8_t test_buckets[1 << 4] = { 0 };
    for (int i = 0; i < 256; i++) {
        char d[4];
        d[0] = d[1] = d[2] = d[3] = i;

        for (int l = 1; l <= 4; l++) {
            hll_add(hll, d, l);
            const uint64_t hash = CityHash64(d, l);
            const size_t bucket = hash & ((1 << 4) - 1);
            const uint8_t nzeros = _hll_count_leading_zeros(hash) + 1;
            test_buckets[bucket] = test_buckets[bucket] < nzeros ? nzeros : test_buckets[bucket];
        }
    }

    printf("estimate: %lu\n", hll_get_estimate(hll));

    for (int i = 0; i < 1 << 4; i++) {
        printf("%d zeros for bucket %d\n", hll->buckets[i], i);
        if (test_buckets[i] != hll->buckets[i]) {
            fprintf(stderr, "%d != %d for bucket %d\n", test_buckets[i], hll->buckets[i], i);
            ret = 1;
        }
    }

    return ret;
}
