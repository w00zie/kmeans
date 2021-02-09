#ifndef KMEANS_UTILS_H
#define KMEANS_UTILS_H

#include <algorithm>
#include "container.h"
#include <iterator>

namespace kmeans {

    template<typename T, const size_t D>
    double calc_distance(const vec<T, D>& p, const vec<T, D>& q) {
        double sum = 0.0;
        for (size_t i = 0; i < D; ++i)
            sum += ((p[i] - q[i]) * (p[i] - q[i]));
        return sum;
    }

    template<typename T, const size_t M, const size_t D>
    bool are_close_to_real(hash<T, D>& centroids, const mat<T, M, D>& real, const double eps_to_real) {
        vec<bool, M> are_close {false};
        for (size_t i = 0; i < M; ++i) {
            for (size_t j = 0; j < M; ++j) {
                if (calc_distance(centroids[i], real[j]) <= eps_to_real)
                    are_close[i] = true;
            }
        }
        return std::all_of(are_close.begin(), are_close.end(), [](const bool b){return b;});   
    }

    template<typename T, const size_t M, const size_t D>
    bool has_converged(hash<T, D>& prev_cent, hash<T, D>& curr_cent, const double eps) {
        vec<double, M> distances;
        for (size_t i = 0; i < M; ++i)
            distances[i] = calc_distance(prev_cent[i], curr_cent[i]);
        return std::all_of(distances.begin(), distances.end(), [eps](double d){return d <= eps;});
    }

    template<typename T, const size_t M, const size_t D>
    size_t get_closest_centroid(const vec<T, D>& point, hash<T, D>& centroids) {
        /* Returns the index of the least distanced element in a matrix wrt a point.
        *
        * Given a data point p this method returns the closest (candidate) cluster center.
        *
        * @param point The 1xD point we want to associate to a (candidate) cluster center. 
        * @param centroids MxD A matrix of cluster centers.
        */
        vec<double, M> distances;
        for (size_t i = 0; i < M; ++i)
            distances[i] = calc_distance(point, centroids[i]);
        auto min_dist = std::min_element(distances.begin(), distances.end());
        return std::distance(distances.begin(), min_dist);
    }

} // namespace kmeans

#endif 
