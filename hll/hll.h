#ifndef _LIBHLL_HLL_H_
#define _LIBHLL_HLL_H_

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/** Opaque HLL data structure
 */
struct hll_s;

/** Opaque HLL data type
 */
typedef struct hll_s hll_t;

/** Create HLL data structure
 *
 * @param bucket_bits - Number of bits to use for the buckets.
 *                      Actual number of buckets will be 2^bucket_bits.
 *                      Must be 4 <= bucket_bits <= 16.
 * @return HLL data type or 0 on error. Error can be memory allocation failure 
 *         or invalid bucket_bits value.
 */
hll_t *hll_create(size_t bucket_bits);

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

/** Get the estimated cardinality based on the data added to the estimator
 * 
 * @param hll - HLL data type
 * @return Estimated cardinality or 0 if hll is NULL
 */
size_t hll_get_estimate(const hll_t *hll);

#ifdef __cplusplus
}
#endif

#endif

