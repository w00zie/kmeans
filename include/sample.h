#ifndef KMEANS_SAMPLE_H
#define KMEANS_SAMPLE_H

#include <algorithm>
#include "container.h"
#include <random>

namespace kmeans::sample {

    template <typename T, const size_t N, const size_t M, const size_t D>
    hash<T, D> sample_centroids_from_data(const mat<T, N, D>& data) {
        /* Random initialization of the centroids. Sampled (without replacement)
        * from the data set.
        * 
        * @param data The NxD data matrix of all points.
        * @return centroids The MxD matrix of randomly sampled centroids.
        */
        static std::random_device seed;
        static std::mt19937 random_number_generator(seed());
        std::vector<size_t> range(N);
        std::iota(range.begin(), range.end(), 0);
        std::shuffle(range.begin(), range.end(), random_number_generator);
        hash<T, D> centroids;
        centroids.reserve(M);
        for (size_t i = 0; i < M; ++i)
            centroids[i] = data[range[i]];
        return centroids;
    }

} // namespace kmeans::sample

#endif