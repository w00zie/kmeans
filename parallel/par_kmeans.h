#ifndef PAR_KMEANS_H
#define PAR_KMEANS_H

#include "../include/container.h"
#include "../include/container_io.h"
#include "../include/utils.h"

#include <algorithm>
#include <iostream>
#include <mutex>
#include <random>
#include "thread_pool.h"

namespace kmeans::par {

    std::mutex mtx;

    template <typename T, const size_t N, const size_t M, const size_t D>
    void assign_subset(const mat<T, N, D>& data, hash<T, D>& centroids,
                                  hash<T, D>& future_centroids, counter& global_counts,
                                  const size_t start, const size_t end) {

        hash<T, D> partial_centroids;
        counter partial_counts;
        partial_centroids.reserve(M);
        partial_counts.reserve(M);

        for (size_t i = start; i < end; ++i) {
            size_t closest_centroid = get_closest_centroid<T, M, D>(data[i], centroids);
            partial_centroids[closest_centroid] = partial_centroids[closest_centroid] + data[i];
            partial_counts[closest_centroid] += 1;
        }
        
        std::lock_guard<std::mutex> lock(mtx);
        for (const auto& c : partial_centroids) {
            future_centroids[c.first] = future_centroids[c.first] + c.second;
            global_counts[c.first] += partial_counts[c.first];
        }
        return;
    }
    
    template <typename T, const size_t N, const size_t M, const size_t D>
    hash<T, D> kmeans(const mat<T, N, D>& data, hash<T, D>& centroids, const size_t niter, const size_t nthreads) {
        kmeans::par::tp::thread_pool pool(nthreads);
        const size_t step = (N + nthreads - 1) / nthreads;
        for (size_t i = 0; i < niter; ++i) {
            hash<T, D> future_centroids;
            counter global_counter;
            future_centroids.reserve(M);
            global_counter.reserve(M);
            for (size_t t = 0; t < N; t += step) {
                size_t real_step = (t + step > N) ? N - t : step;
                pool.enqueue([&, t, real_step](){
                    assign_subset<T, N, M, D>(data, 
                                              centroids, 
                                              future_centroids, 
                                              global_counter,
                                              t, 
                                              t+real_step);
                });
            }
            pool.wait_all_threads();
            for (const auto& fc : future_centroids)
                centroids[fc.first] = fc.second / global_counter[fc.first];
        }
        return centroids;
    }

    template <typename T, const size_t N, const size_t M, const size_t D>
    hash<T, D> kmeans(const mat<T, N, D>& data, hash<T, D>& centroids, 
                      const size_t niter, const double eps, const size_t nthreads) {
        kmeans::par::tp::thread_pool pool(nthreads);
        const size_t step = (N + nthreads - 1) / nthreads;
        for (size_t i = 0; i < niter; ++i) {
            hash<T, D> future_centroids;
            counter global_counter;
            future_centroids.reserve(M);
            global_counter.reserve(M);
            for (size_t t = 0; t < N; t += step) {
                size_t real_step = (t + step > N) ? N - t : step;
                pool.enqueue([&, t, real_step](){
                    assign_subset<T, N, M, D>(data, 
                                              centroids, 
                                              future_centroids, 
                                              global_counter,
                                              t, 
                                              t+real_step);
                });
            }
            pool.wait_all_threads();
            for (auto fc = future_centroids.begin(); fc != future_centroids.end(); ++fc)
                fc.value() = fc.value() / global_counter[fc.key()];
            if (has_converged<T, M, D>(centroids, future_centroids, eps)) {
                std::cout << "\nConverged at " << i << "th iteration.\n";
                return future_centroids;
            }
            centroids = future_centroids;
        }
        return centroids;
    }

} // namespace kmeans::par

#endif