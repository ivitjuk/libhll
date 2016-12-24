# LibHLL - A HyperLogLog C library

This is a C implementaion of the HyperLogLog cardinality estimator.

More documentation on the estimator can be found by following the links:

* [Original Paper](http://algo.inria.fr/flajolet/Publications/FlFuGaMe07.pdf)
* [Google Paper](http://static.googleusercontent.com/media/research.google.com/en//pubs/archive/40671.pdf)
* [Neustar Blog](https://research.neustar.biz/2012/10/25/sketch-of-the-day-hyperloglog-cornerstone-of-a-big-data-infrastructure/)

# API

For details see [hll.h](https://github.com/ivitjuk/libhll/blob/master/hll/hll.h).

* `hll_create()` - Create HLL data structure
* `hll_release()` - Release HLL type previously allocated with hll_create()
* `hll_add()` - Add a sample to the HLL estimator
* `hll_get_estimate()` - Get the estimated cardinality based on the data added to the estimator

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
