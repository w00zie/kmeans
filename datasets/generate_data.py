import numpy as np
from sklearn.datasets import make_blobs
from matplotlib import pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
from typing import Tuple
import os

def generate_data(n_samples: int,
                  n_dims: int,
                  n_clusters: int,
                  std: float,
                  seed: int = 23) -> Tuple[np.ndarray,
                                           np.ndarray,
                                           np.ndarray]:
    data, labels, centers = \
            make_blobs(n_samples=n_samples,
                       n_features=n_dims,
                       centers=n_clusters,
                       cluster_std=std,
                       random_state=seed,
                       shuffle=True,
                       return_centers=True)
    return (data, labels, centers)

def plot_2d_data(data: np.ndarray,
                 centers: np.ndarray) -> None:
    assert data.ndim == centers.ndim == 2
    x, y = data[:,0], data[:, 1]
    plt.scatter(x, y, c="b")
    for cx, cy in centers:
        plt.scatter(cx, cy, c="r")
    plt.show()
    plt.clf()

def plot_3d_data(data: np.ndarray,
                 centers: np.ndarray) -> None:
    assert data.ndim == centers.ndim == 2
    fig = plt.figure()
    ax = fig.add_subplot(111, projection='3d')
    x, y, z = data[:,0], data[:,1], data[:,2]
    ax.scatter(x, y, z, marker="o", s=0.5)
    for cx, cy, cz in centers:
        ax.scatter(cx, cy, cz, c="r", marker="^")
    plt.show()
    plt.clf()

if __name__ == "__main__":

    N_SAMPLES = 500000
    N_DIMS = 2
    N_CLUSTERS = 3
    STD = 3.
    PLT = False
    DATASET_PATH = "."
    data, labels, centers = generate_data(n_samples=N_SAMPLES,
                                          n_dims=N_DIMS,
                                          n_clusters=N_CLUSTERS,
                                          std=STD)
    print(centers)
    if N_DIMS == 2:
        sub_dir = os.path.join(DATASET_PATH, "2d")
        if PLT:
            plot_2d_data(data, centers)
    elif N_DIMS == 3:
        sub_dir = os.path.join(DATASET_PATH, "3d")
        if PLT:
            plot_3d_data(data, centers)
    sub_dir = os.path.join(sub_dir, f"{N_SAMPLES}_samples_{N_CLUSTERS}_centers")
    if not os.path.exists(sub_dir):
        os.mkdir(sub_dir)
    out_path_data = os.path.join(sub_dir, "data.csv")
    out_path_centroids = os.path.join(sub_dir, "centroids.csv")
    np.savetxt(out_path_data, data, delimiter=",")
    np.savetxt(out_path_centroids, centers, delimiter=",")
