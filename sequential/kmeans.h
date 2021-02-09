#ifndef KMEANS_H
#define KMEANS_H

#include <algorithm>
#include "../include/container.h"
#include "../include/utils.h"
#include <iostream>

namespace kmeans::seq {

    template <typename T, const size_t N, const size_t M, const size_t D>
    hash<T, D> assign_and_update(const mat<T, N, D>& data, hash<T, D>& centroids) {
        hash<T, D> new_centroids;
        counter counts;
        new_centroids.reserve(M);
        counts.reserve(M);
        // Assignment step
        for (size_t i = 0; i < N; ++i) {
            size_t closest_centroid = get_closest_centroid<T, M, D>(data[i], centroids);
            new_centroids[closest_centroid] = new_centroids[closest_centroid] + data[i];
            counts[closest_centroid] += 1;
        }
        // Update step
        for (size_t i = 0; i < M; ++i)
            new_centroids[i] = new_centroids[i] / counts[i];
        return new_centroids;
    }


    template <typename T, const size_t N, const size_t M, const size_t D>
    hash<T, D> kmeans(const mat<T, N, D>& data, hash<T, D>& centroids, const size_t niter) {
        /* Running the assignment step followed by the update step for a fixed number of iterations.
        *
        * @param data The NxD data matrix of all points.
        * @param centroids The MxD matrix of the initials cluster centers.
        * @param niter The total number of iterations.
        * @return centroids The MxD matrix of finals cluster centers.
        */
        for (size_t i = 0; i < niter; ++i)
            centroids = assign_and_update<T, N, M, D>(data, centroids);
        return centroids;
    }

    template <typename T, const size_t N, const size_t M, const size_t D>
    hash<T, D> kmeans(const mat<T, N, D>& data, hash<T, D>& centroids, const size_t niter, const double eps) {
        /* Running the assignment step followed by the update step for a fixed number of iterations.
        *
        * Centroids are returned when their euclidean distance wrt to the previous candidates
        * is lower then the threshold `eps`. If this condition never occurs the algorithm
        * goes on for `niter`.
        * 
        * @param data The NxD data matrix of all points.
        * @param centroids The MxD matrix of the initials cluster centers.
        * @param niter The total number of iterations.
        * @param eps Tolerance for establishing the convergence.
        * @return centroids The MxD matrix of finals cluster centers.
        */
        for (size_t i = 0; i < niter; ++i) {
            hash<T, D> current_cent = assign_and_update<T, N, M, D>(data, centroids);
            if (has_converged<T, M, D>(centroids, current_cent, eps)) {
                std::cout << "Converged at " << i << "th iteration.\n";
                return current_cent;
            }
            centroids = current_cent;
        }
        return centroids;
    }

} // namespace kmeans::seq

#endif