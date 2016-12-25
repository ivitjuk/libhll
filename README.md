# LibHLL - A HyperLogLog C library

This is a C implementation of the HyperLogLog cardinality estimator.

This library implements the algorithm as described in the original paper [Original Paper](http://algo.inria.fr/flajolet/Publications/FlFuGaMe07.pdf). More information can be found on the [Neustar Blog](https://research.neustar.biz/2012/10/25/sketch-of-the-day-hyperloglog-cornerstone-of-a-big-data-infrastructure/).

There is an [Google](http://static.googleusercontent.com/media/research.google.com/en//pubs/archive/40671.pdf) improvement for the algorithm for estimating large cardinalities. It is not implemented in this library.

# API

For details see [hll.h](https://github.com/ivitjuk/libhll/blob/master/hll/hll.h).

* `hll_create()` - Create HLL data structure
* `hll_release()` - Release HLL type previously allocated with hll_create()
* `hll_add()` - Add a sample to the HLL estimator
* `hll_get_estimate()` - Get the estimated cardinality based on the data added to the estimator

# Build

```
git clone https://github.com/ivitjuk/libhll.git
cd libhll
cmake .
make
make test
cat /usr/share/dict/words | ./hll_demo $(wc -l < /usr/share/dict/words)
```

# Example

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

printf("%lu\n", hll_get_estimate(hll));

hll_release(hll);

```
