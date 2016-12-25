# LibHLL - A HyperLogLog C library

This is a C implementation of the HyperLogLog cardinality estimator.

This library implements the algorithm as described in the [Original Paper](http://algo.inria.fr/flajolet/Publications/FlFuGaMe07.pdf). More information can be found on the [Neustar Blog](https://research.neustar.biz/2012/10/25/sketch-of-the-day-hyperloglog-cornerstone-of-a-big-data-infrastructure/).

There is the [Google](http://static.googleusercontent.com/media/research.google.com/en//pubs/archive/40671.pdf) improvement for the algorithm for estimating large cardinalities. It is not implemented in this library.

## Features

* Implements the algorithm from the original paper
* Can use custom hash function
* Provides merge operation
* `hll_estimate_t` type includes internals of the estimator state

## API

For details see [hll.h](https://github.com/ivitjuk/libhll/blob/master/hll/hll.h).

* `hll_create()` - Create HLL data structure
* `hll_release()` - Release HLL type previously allocated with hll_create()
* `hll_add()` - Add a sample to the HLL estimator
* `hll_get_estimate()` - Get the estimated cardinality based on the data added to the estimator
* `hll_merge()` - Merge data from two HLLs
* `hll_set_hash_function()` - Change the hash function to be used to the estimator

## Build

```
git clone https://github.com/ivitjuk/libhll.git
cd libhll
cmake .
make
make test
cat /usr/share/dict/words | ./hll_demo $(wc -l < /usr/share/dict/words)
```

## Example

For a full example see [hll_demo.c](https://github.com/ivitjuk/libhll/blob/master/hll_demo.c).

```
hll_t hll;
hll = hll_create(14);

char *line = NULL;
size_t buflen = 0;
size_t linelen = 0;

while ((linelen = getline(&line, &buflen, stdin)) != -1) {
    hll_add(hll, line, linelen-1);
}

hll_estimate_t estimate;
hll_get_estimate(hll, estimate);
printf("%lu\n", estimate.estimate);

hll_release(hll);

```
