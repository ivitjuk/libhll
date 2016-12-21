#include <hll/hll.h>
#include <hll/hll_private.h>

#include <stdlib.h>
#include <string.h>

hll_t *hll_create()
{
    hll_t *hll = (hll_t *)malloc(sizeof(hll_t));
    if (!hll) {
        goto err;    
    }

    memset(hll, 0, sizeof(hll_t));


err:
    return hll;
}

void hll_release(hll_t *hll)
{
    free(hll);
}

void hll_add(const hll_t *hll, unsigned char *data, size_t data_len)
{
}

hll_result_t *hll_result_get(const hll_t *hll)
{
    return 0;
}

void hll_result_release(hll_result_t *hll_result)
{
}

