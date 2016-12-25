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

int main(int argc, const char *argv[])
{
    int ret = 0;

    hll_t *hll;
    hll_estimate_t estimate;

    /*
     * Zero estimate
     */
    printf("Testing zero estimate\n");
    hll = hll_create(4);
    hll_get_estimate(hll, &estimate);
    if(estimate.estimate != 0) {
        fprintf(stderr, "failed zero estimate: %lu != 0\n", estimate.estimate);
        ret = 1;
    }

    /*
     * No correction
     */
    printf("Testing no range correction\n");
    static const size_t expected[] = {
        1, 2, 3, 4, 5, 7, 9, 11, 13, 15, 18, 42, 56, 83, 162, 2756
    };

    for (int i = 0; i < 16; i++) {
        hll->buckets[i] = 8;
        hll_get_estimate(hll, &estimate);
        if(estimate.estimate != expected[i]) {
            ret = 1;
            fprintf(stderr, "failed estimate [%d]: %lu != %lu\n", i, estimate.estimate, expected[i]);
        }
    }

    hll_reset(hll);

    /*
     * Small range correction
     */
    printf("Testing small range correction\n");
    static const size_t expected_small_range[] = {
        1, 2, 3, 4, 5, 7, 9, 11, 13, 15, 18, 22, 26, 33, 44, 21
    };

    for (int i = 0; i < 16; i++) {
        hll->buckets[i] = 1;
        hll_get_estimate(hll, &estimate);
        if(estimate.estimate != expected_small_range[i]) {
            ret = 1;
            fprintf(stderr, "failed small range estimate [%d]: %lu != %lu\n", i, estimate.estimate, expected_small_range[i]);
        }
    }

    hll_reset(hll);

    /*
     * Large range correction
     */
    printf("Testing large range correction\n");
    for (int i = 0; i < 16; i++) {
        hll->buckets[i] = 25;
    }

    hll_get_estimate(hll, &estimate);
    if(estimate.estimate != 377421910) {
        ret = 1;
        fprintf(stderr, "failed large range estimate: %lu != %lu\n", estimate.estimate, 0UL);
    }

    hll_release(hll);

    return ret;
}
