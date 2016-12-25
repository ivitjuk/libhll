/*
  ISC License (ISC)

  Copyright (c) 2016-2017, Ivan Vitjuk <virtus@eclipsedminds.org>

  Permission to use, copy, modify, and/or distribute this software for any
  purpose with or without fee is hereby granted, provided that the above
  copyright notice and this permission notice appear in all copies.

  THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH
  REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND
  FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT,
  INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
  LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
  OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
  PERFORMANCE OF THIS SOFTWARE.
*/

#include <hll/hll.h>
#include <hll/hll_private.h>

#include <city/city.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define HLL_MAX(a, b) ((a) > (b) ? (a) : (b))

#undef dprintf
#undef DEBUG_PRINTF

#ifdef DEBUG_PRINTF
#define dprintf(...) fprintf(stderr, __VA_ARGS__)
#else
#define dprintf(...)
#endif

static uint8_t _leading_zeros_table[256];

hll_t *hll_create(size_t bucket_bits)
{
    if (_leading_zeros_table[0] == 0) {
        _hll_init_leading_zeros_table();
    }
    
    hll_t *hll = 0;

    if (bucket_bits < 4 || bucket_bits > 16) {
        goto err;
    }

    const size_t n_buckets = 1 << bucket_bits;
    const size_t space_needed = sizeof(hll_t) + sizeof(uint8_t) * n_buckets;

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
            hll->alpha = 0.7213 / (1.0 + 1.079 / (double)hll->n_buckets);
    }
   
err:
    return hll;
}

void hll_reset(hll_t *hll)
{
    if (!hll) {
        return;
    }

    memset(hll->buckets, 0, sizeof(hll->buckets[0]) * hll->n_buckets);
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

    // Per original paper, we need 32bits;
    const uint32_t hash = CityHash64(data, data_len) % (1UL << 32); 
    const size_t bucket = hash & (hll->n_buckets - 1);
    const uint8_t nzeros = _hll_count_leading_zeros(hash | (hll->n_buckets - 1)) + 1;
    hll->buckets[bucket] = HLL_MAX(hll->buckets[bucket], nzeros);

    dprintf("hash: %u, bucket: %lu, nzeros+1: %d\n", hash, bucket, nzeros);
}

int hll_get_estimate(const hll_t *hll, hll_estimate_t *estimate)
{
    if (!hll || !estimate) {
        return 0;
    }

    memset(estimate, 0, sizeof(*estimate));
    estimate->alpha = hll->alpha;
    estimate->n_buckets = hll->n_buckets;
    double sum = 0;

    for (size_t i = 0; i < hll->n_buckets; i++) {
        sum += 1.0 / (double)(1UL << hll->buckets[i]);
        if (hll->buckets[i] == 0) {
            estimate->n_empty_buckets++;
        }
    }

    estimate->hll_estimate = hll->alpha * hll->n_buckets * hll->n_buckets / sum;
    estimate->estimate = estimate->hll_estimate;

    if (estimate->hll_estimate < hll->n_buckets * 5.0 / 2.0) {
        if (estimate->n_empty_buckets > 0) {
            dprintf("small range correction: %lu < %f (n_empty_buckets: %lu)\n", 
                    estimate->hll_estimate, hll->n_buckets * 5.0 / 2.0, estimate->n_empty_buckets);

            estimate->small_range_estimate = -(double)hll->n_buckets * log((double)estimate->n_empty_buckets / (double)hll->n_buckets);
            estimate->estimate = estimate->small_range_estimate;
        }
    } else if (estimate->hll_estimate > (1UL << 32) / 30) {
        dprintf("large range correction: %lu > %lu\n", estimate->hll_estimate, (1UL << 32) / 30);

        estimate->large_range_estimate = -(double)(1UL << 32) * log((double)1.0 - (double)estimate->hll_estimate / (double)(1UL << 32));
        estimate->estimate = estimate->large_range_estimate;
    }

    dprintf("result: alpha=%f, n_buckets=%lu, n_empty_buckets=%lu, "
            "estimate=%lu, hll_estimate=%lu, small_range_estimate=%lu, large_range_estimate=%lu\n", 
            estimate->alpha,
            estimate->n_buckets,
            estimate->n_empty_buckets,
            estimate->estimate,
            estimate->hll_estimate,
            estimate->small_range_estimate,
            estimate->large_range_estimate);

    return 1;
}

int hll_merge(const hll_t *hll1, const hll_t *hll2)
{
    if (hll1->n_buckets != hll2->n_buckets) {
        return 0;
    }

    for (int i = 0; i < hll1->n_buckets; i++) {
        hll1->buckets[i] = HLL_MAX(hll1->buckets[i], hll2->buckets[i]);
    }

    return 1;
}

uint8_t _hll_count_leading_zeros(uint32_t hash)
{
    uint8_t nzeros = 0;

    for(int i = 24; i >= 0; i -= 8) {
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

