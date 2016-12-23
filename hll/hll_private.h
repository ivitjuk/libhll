#ifndef _LIBHLL_HLL_PRIVATE_H_
#define _LIBHLL_HLL_PRIVATE_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

struct hll_s {
    double alpha;
    size_t n_buckets;
    uint8_t *buckets;
};

uint8_t _hll_count_leading_zeros(uint64_t hash);
void _hll_init_leading_zeros_table();

#ifdef __cplusplus
}
#endif

#endif

