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
#include <stdint.h>

static uint64_t test_hash(const char *data, size_t hashvalue)
{
    return hashvalue;
}

int main(int argc, const char *argv[])
{
    int ret = 0;

    hll_t *hll = hll_create(4);
    hll_set_hash_function(hll, test_hash);
    
    for (int bucket = 0; bucket < 16; bucket++) {
        for (int i = 4; i < 32; i++) {
            hll_add(hll, "  ",  (1 << i) | bucket);
            for (int b = 0; b < hll->n_buckets; b++) {
                /* printf("%2d:%2d; ", b, hll->buckets[b]); */
                if (b == bucket) {
                    if (hll->buckets[b] != 32-i) {
                        fprintf(stderr, "failure: %d %d %d (val:%d)\n", bucket, i, b, hll->buckets[b]);
                        ret = 1;
                    }
               } else {
                    if (hll->buckets[b] != 0) {
                        fprintf(stderr, "failure: %d %d %d (val:%d)\n", bucket, i, b, hll->buckets[b]);
                        ret = 1;
                    }
                }
            }
            /* printf("\n"); */
            hll_reset(hll);
        }
    }

    hll_release(hll);

    return ret;
}
