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

#include <stdio.h>
#include <stdint.h>

int main(int argc, const char *argv[])
{
    int ret = 0;

    _hll_init_leading_zeros_table();
    for(int i = 0; i < 32; i++) {
        const uint32_t val = 1lu << i;
        const size_t nzeros = _hll_count_leading_zeros(val);
        const size_t expected = 32 - i - 1;
        // printf("%lu: %lu\n", val, nzeros);
        if (nzeros != expected) {
            fprintf(stderr, "%lu != %lu (for %u)\n", nzeros, expected, val);
            ret  = 1;
        }
    }
    
    return ret;
}
