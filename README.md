# k-means clustering

![](https://i.imgur.com/S65Sk9c.jpg)

## Brief description

k-means is a simple and popular clustering technique. It is a standard baseline when the number of cluster centers (*k*) is known (or almost known) a-priori.

> Given a set of observations (*x1, x2, ..., xn*), where each observation is a *d*-dimensional real vector, *k*-means clustering aims to partition the *n* observations into *k <= n* sets so as to minimize the within-cluster sum of squares (i.e. variance) [[1]]. 

In its naive form (also known as the Lloyd's version) the algorithm is composed of two steps:

- **Assignment step**: Assign each observation to the cluster with the nearest mean: that is the one with the least squared Euclidean distance.

- **Update step**: Recalculate means (centroids) for observations assigned to each cluster. 

The average complexity is given by *O(knT)*, were *n* is the number of samples and *T* is the number of iteration.

Although being a computationally hard problem (NP-hard) this algorithm is, in practice, very fast, but it falls in local minima. Given these properties it is usually restarted several times, in order to get a better estimate of the real centroids.

Fore more details have a look at my [report](report.pdf).

## Code

k-means works for Euclidean spaces of arbitrary dimensionality (obviously suffering the [curse of dimensionality](https://en.wikipedia.org/wiki/Curse_of_dimensionality)) and in this implementation this number will be not fixed a-priori. 

A few simple synthetic datasets of various dimensionalities (2 and 3) and data size (500 to 500k data points) are placed under `datasets/`. For example 

```
./datasets/2d/100000_samples_3_centers
```

is a directory containing a dataset of 100000 points in 2D synthetically generated around 3 cluster centers (centroids). Inside every directory there will be 2 `csv` files:

1. `centroids.csv`: The real centroids (that should match the ones computed by k-means).
2. `data.csv`: The actual data to be clustered.

The code is structured as following:
- `include/`: Contains the headers used by both versions (sequential and parallel). **IMPORTANT REMARK**: The following four header files are taken from [robin_map](https://github.com/Tessil/robin-map), in particular from the commit `84c1bee16e2449c28589ccd6ab366df257d18c24`. These implement a faster version of the `std::unordered_map`. Copyright notice has been mantained, as required by the MIT license.

    1. `include/robin_growth_policy.h`
    2. `include/robin_hash.h`
    3. `include/robin_map.h`
    4. `include/robin_hash.h`

    Other than that this sub-directory contains:
    - `container.h`: Header file implementing the definitions of the data structures mainly used in this project. These are simply arrays, arrays of arrays (namely `vec` and `mat`) and hash maps. If you don't want to use `robin_map`, just change the alias definition of the `kmeans::hash` at *line 18*. You can change it with the `std::unordered_map` but be aware that doing so will slow down the code.
    - `container_io.h`: Header file implementing a few helper functions for I/O.
    - `utils.h`: Header file implementing a few methods used by both versions (sequential and parallel).
    - `sample.h`: Header file for the sampling process (will be included in `utils.h` in a future commit).

This implementation passes [cppcheck](https://github.com/danmar/cppcheck) and [valgrind](https://valgrind.org/) tests.

## Sequential

Under `sequential/` you will find:

- `benchmark/`: Sub-directory used for running benchmarks.
- `test/`: Sub-directory used for (*sort of*) unit-testing the algorithm. Has a separate README file.
- `kmeans.h`: Header file containing the implementation of the sequential kmeans algorithm.
- `Makefile`: Very (very) simple makefile needed just for alternating between debug and release mode. 
- `seq.cpp`: Example source file where the code functionality is shown.

### Running

Since this implementation is centered on **performance** the user has to specify every time the "*hyperparameters*" of the algorithm, in order to help the compiler optimize the code. This is done in the `main` function inside `seq.cpp`, where these lines have to be edited each time

```cpp
const size_t num_points = 100000;
const size_t dim = 2;
const size_t k = 3;
const size_t niter = 500;
const std::string data_path = "../datasets/2d/100000_samples_3_centers/data.csv";
const double eps = 1e-4;
```

In particular

- `num_points` is the number of data points (**Warning**: this number needs to match the one preceding `_samples` in the `data_path` variable).
- `dim` is the dimensionality of the data set (**Warning**: this number needs to match the one indicating the sub-directory in the `data_path` variable).
- `k` is the number of cluster centers (**Warning**: this number needs to match the one preceding `_centers` in the `data_path` variable).
- `niter` is the number of iterations that the algorithm will run through.
- `data_path` is the string containing the path to the `data.csv` that the user wants to cluster.
- `eps` is the tolerance value for establishing the convergence of the algorithm. When all the candidate centroids have moved of a distance <= eps wrt the previous candidates, then the algorithm will stop. If set to a value less or equal to zero this feature will not be used and the algorithm will run for `niter`.

Once these are set up the user has to run

```
$ make
$ ./kmeans_seq
```
and the algorithm will execute. In this early version the centroids computed by k-means will be printed to the console. The main function (`kmeans::seq::kmeans`) will be timed and the elapsed time will be displayed as well.

For debugging run
```
$ make sequential_db
$ ./kmeans_seq_db
```

The compiler I used is [g++](https://man7.org/linux/man-pages/man1/g++.1.html) 10.2.0 with `-std=c++17`.

## Parallel

This version does not differ substantially from the sequential one.

Under `parallel/` you will find:

- `benchmark/`: Sub-directory used for running benchmarks.
- `test/`: Sub-directory used for (*sort of*) unit-testing the algorithm. Has a separate README file.
- `par_kmeans.h`: Header file containing the implementation of a multi-threaded version of the kmeans algorithm.
- `thread_pool.h`: Header file containing the implementation of a thead pool.
- `Makefile`: Very (very) simple makefile needed just for alternating between debug and release mode. 
- `seq.cpp`: Example source file where the code functionality is shown.

### Running

You can refer to the previous section for instructions: specifications are the same (except that you can now set the desired number of threads).

---

[1]: https://en.wikipedia.org/wiki/K-means_clustering
