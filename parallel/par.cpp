#include <chrono>
#include "../include/container_io.h"
#include "par_kmeans.h"
#include "../include/sample.h"
#include "../include/utils.h"

int main() {

    const size_t num_points = 5000;
    const size_t dim = 3;
    const size_t k = 3;
    const size_t niter = 10;
    const std::string data_path = "../datasets/3d/5000_samples_3_centers/data.csv";
    const std::string centroids_path = "../datasets/3d/5000_samples_3_centers/centroids.csv";
    const double eps = 0;
    const double eps_to_real = 2;
    const size_t nthreads = 4;

    std::cout << "\n======================== k-means ========================\n";
    std::cout << "Dataset: " << data_path << '\n';
    std::cout << "Threads: " << nthreads << '\n';

    const kmeans::mat<float, num_points, dim> data = kmeans::io::load_csv<float, num_points, dim>(data_path, ',');
    kmeans::hash<float, dim> sampled_centroids = kmeans::sample::sample_centroids_from_data<float, num_points, k, dim>(data);
    if (eps > 0) {
        auto start = std::chrono::high_resolution_clock::now();
        kmeans::hash<float, dim> centroids = kmeans::par::kmeans<float, num_points, k, dim>(data, sampled_centroids, niter, eps, nthreads);    
        auto end = std::chrono::high_resolution_clock::now();

        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        std::cout << "\nParallel Duration: " << duration << " ms" << "\n\n";
        kmeans::io::print_centroids(centroids);

        const kmeans::mat<float, k, dim> real_centroids = kmeans::io::load_csv<float, k, dim>(centroids_path, ',');
        std::cout << "\nChecking if the calculated centroids are sufficiently close to the real ones...\n";
        if(kmeans::are_close_to_real<float, k, dim>(centroids, real_centroids, eps_to_real))
            std::cout << "....OK!\n";
        else
            std::cout << "....KO!\n";
    } else {
        auto start = std::chrono::high_resolution_clock::now();
        kmeans::hash<float, dim> centroids = kmeans::par::kmeans<float, num_points, k, dim>(data, sampled_centroids, niter, nthreads);
        auto end = std::chrono::high_resolution_clock::now();

        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        std::cout << "\nParallel Duration: " << duration << " ms" << "\n\n";
        kmeans::io::print_centroids(centroids);

        const kmeans::mat<float, k, dim> real_centroids = kmeans::io::load_csv<float, k, dim>(centroids_path, ',');
        std::cout << "\nChecking if the calculated centroids are sufficiently close to the real ones...\n";
        if(kmeans::are_close_to_real<float, k, dim>(centroids, real_centroids, eps_to_real))
            std::cout << "....OK!\n";
        else
            std::cout << "....KO!\n";
    }

    std::cout << "==========================================================\n";

    return 0;
}
