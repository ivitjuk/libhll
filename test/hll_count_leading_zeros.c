#include <hll/hll.h>
#include <hll/hll_private.h>

#include <stdio.h>
#include <stdint.h>

int main(int argc, const char *argv[])
{
    int ret = 0;

    hll_init_lib();
    for(int i = 0; i < 64; i++) {
        const uint64_t val = 1lu << i;
        const size_t nzeros = _hll_count_leading_zeros(val);
        const size_t expected = 64 - i - 1;
        // printf("%lu: %lu\n", val, nzeros);
        if (nzeros != expected) {
            fprintf(stderr, "%lu != %lu (for %lu)\n", nzeros, expected, val);
            ret  = 1;
        }
    }
    
    return ret;
}
