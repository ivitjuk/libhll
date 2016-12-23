#include <hll/hll.h>
#include <hll/hll_private.h>

#include <city/city.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define NDEBUG

#define HLL_MAX(a, b) ((a) > (b) ? (a) : (b))

#ifdef DEBUG
#define dprintf(...) printf(__VA_ARGS__)
#else
#define dprintf(...)
#endif

static uint8_t _leading_zeros_table[256];

void hll_init_lib()
{
    _hll_init_leading_zeros_table();
}

hll_t *hll_create(size_t bucket_bits)
{
    hll_t *hll = 0;

    if (bucket_bits < 4 || bucket_bits > 16) {
        goto err;
    }

    const size_t n_buckets = 1 << bucket_bits;

    const size_t space_needed = 
        sizeof(hll_t) + 
        sizeof(uint8_t) * n_buckets;

    hll = (hll_t *)malloc(space_needed);
    if (!hll) {
        goto err;    
    }

    memset(hll, 0, space_needed);

    hll->buckets = (uint8_t *)((unsigned char *)hll + sizeof(hll_t));
    hll->n_buckets = n_buckets;

    switch(hll->n_buckets) {
        case 16:
            hll->alpha = 0.673;
            break;
        case 32:
            hll->alpha = 0.697;
            break;
        case 64:
            hll->alpha = 0.709;
            break;
        default:
            hll->alpha = 0.7213 / (1.0 + 1.079 / hll->n_buckets);
    }
    
err:
    return hll;
}

void hll_release(hll_t *hll)
{
    free(hll);
}

void hll_add(const hll_t *hll, const char *data, size_t data_len)
{
    if (!hll) {
        return;
    }

    const uint64_t hash = CityHash64(data, data_len);
    const size_t bucket = hash & (hll->n_buckets - 1);
    const uint8_t nzeros = _hll_count_leading_zeros(hash) + 1;
    hll->buckets[bucket] = HLL_MAX(hll->buckets[bucket], nzeros);

    dprintf("hash: %lu, bucket: %lu, nzeros+1: %d\n", hash, bucket, nzeros);
}

size_t hll_get_estimate(const hll_t *hll)
{
    double sum = 0;

    for (size_t i = 0; i < hll->n_buckets; i++) {
        sum += 1.0 / (1 << hll->buckets[i]);
    }

    return hll->alpha * hll->n_buckets * hll->n_buckets / sum;
}

uint8_t _hll_count_leading_zeros(uint64_t hash)
{
    uint8_t nzeros = 0;

    for(int i = 56; i >= 0; i -= 8) {
        const uint8_t current_byte = (hash >> i) & 0xFF;
        nzeros += _leading_zeros_table[current_byte];
        if (current_byte != 0) {
            break;
        }
    }

    return nzeros;
}

void _hll_init_leading_zeros_table() 
{
    for (int i = 0; i < 256; i++) {
        _leading_zeros_table[i] = 8 - log(i+1) / log(2);
    }
}

