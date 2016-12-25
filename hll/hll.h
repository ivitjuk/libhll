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

#ifndef _LIBHLL_HLL_H_
#define _LIBHLL_HLL_H_

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/** Opaque HLL data structure
 */
struct hll_s;

/** Opaque HLL data type
 */
typedef struct hll_s hll_t;

/** Estimation result data structure
 */
struct hll_estimate_s {
    /** Alpha */
    double alpha;
    /** Number of buckets */
    uint16_t n_buckets;
    /** Number of empty buckets */
    uint16_t n_empty_buckets;
    /** Final estimated cardinality */
    uint64_t estimate;
    /** HLL estimated cardinaloty, before any correction */
    uint64_t hll_estimate;
    /** Small range estimated cardinality */
    uint64_t small_range_estimate;
    /** Large range estimated cardinality */
    uint64_t large_range_estimate;
};

/** Estimation result type
 */
typedef struct hll_estimate_s hll_estimate_t;

/** Create HLL data structure
 *
 * @param bucket_bits - Number of bits to use for the buckets.
 *                      Actual number of buckets will be 2^bucket_bits.
 *                      Must be 4 <= bucket_bits <= 16.
 * @return HLL data type or 0 on error. Error can be memory allocation failure 
 *         or invalid bucket_bits value.
 */
hll_t *hll_create(size_t bucket_bits);

/** Reset state of the estimator
 *
 * @param hll - HLL data type
 */
void hll_reset(hll_t *hll);

/** Release HLL type previously allocated with hll_create().
 *
 * @param hll - HLL data type
 */
void hll_release(hll_t *hll);

/** Add a sample to the HLL estimator
 *
 * @param hll - HLL data type
 * @param data - Sample to be added to the estimator (underlying data type is not important)
 * @param data_len - Lenght of the data sample in bytes
 */
void hll_add(const hll_t *hll, const char *data, size_t data_len);

/** Merge data from two HLL types
 *
 * Data from hll2 will be merged into hll2
 *
 * @param hll1 - First HLL data type
 * @param hll2 - Second HLL data type
 * @return 1 on success, 0 on failure. Fails when number of buckets are not compatible.
 */
int hll_merge(const hll_t *hll1, const hll_t *hll2);

/** Get the estimated cardinality based on the data added to the estimator
 * 
 * @param hll - HLL data type
 * @param estimate - Result of the estimation
 * @return 1 on success, 0 on failure. Fails only on NULL input parameters.
 */
int hll_get_estimate(const hll_t *hll, hll_estimate_t *estimate);

#ifdef __cplusplus
}
#endif

#endif

