#ifndef _LIBHLL_HLL_H_
#define _LIBHLL_HLL_H_

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

struct hll_s;

typedef struct hll_s hll_t;

hll_t *hll_create(size_t bucket_bits);

void hll_release(hll_t *hll);

void hll_add(const hll_t *hll, const char *data, size_t data_len);

size_t hll_get_estimate(const hll_t *hll);

#ifdef __cplusplus
}
#endif

#endif

