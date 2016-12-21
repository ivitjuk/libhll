#ifndef _LIBHLL_HLL_H_
#define _LIBHLL_HLL_H_

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

struct hll;

typedef struct hll hll_t;

struct hll_result {
    int dummy;
};

typedef struct hll_result hll_result_t;

hll_t *hll_create();

void hll_release(hll_t *hll);

void hll_add(const hll_t *hll, unsigned char *data, size_t data_len);

hll_result_t *hll_result_get(const hll_t *hll);

void hll_result_release(hll_result_t *hll_result);

#ifdef __cplusplus
}
#endif

#endif

