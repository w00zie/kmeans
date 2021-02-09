#include <cassert>
#include <chrono>
#include "../../../../include/container_io.h"
#include "../../../par_kmeans.h"
#include "../../../../include/sample.h"

bool run_kmeans() {

    const size_t num_points = 5000;
    const size_t dim = 3;
    const size_t k = 3;
    const size_t niter = 100;
    const std::string data_path = "../../../../datasets/3d/5000_samples_3_centers/data.csv";
    const std::string centroids_path = "../../../../datasets/3d/5000_samples_3_centers/centroids.csv";
    const double eps = 1e-4;
    const double eps_to_real = 2;
    const size_t nthreads = 4;

    const kmeans::mat<float, num_points, dim> data = kmeans::io::load_csv<float, num_points, dim>(data_path, ',');
    kmeans::hash<float, dim> sampled_centroids = kmeans::sample::sample_centroids_from_data<float, num_points, k, dim>(data);
    kmeans::hash<float, dim> centroids = kmeans::par::kmeans<float, num_points, k, dim>(data, sampled_centroids, niter, eps, nthreads);    
    const kmeans::mat<float, k, dim> real_centroids = kmeans::io::load_csv<float, k, dim>(centroids_path, ',');
    return(kmeans::are_close_to_real<float, k, dim>(centroids, real_centroids, eps_to_real));

    return 0;
}

int main(int argc, char *argv[]) {

    const unsigned int NUM_RUNS = std::stoul(argv[1]);
    const unsigned int SUCCESS_THRESHOLD = std::stoul(argv[2]);
    unsigned int OK = 0;
    for (unsigned int i = 0; i < NUM_RUNS; ++i) {
        if ( run_kmeans() )
            OK += 1;
    }
    
    assert(OK > SUCCESS_THRESHOLD);

    return 0;
}