import numpy as np
import pandas as pd
from matplotlib import pyplot as plt
import os

def plot_everything(path):
    data_path = os.path.join(path, "data.csv")
    centr_path = os.path.join(path, "centroids.csv")

    data = pd.read_csv(data_path, sep=',', header=None, names=['x', 'y'])
    real = pd.read_csv(centr_path, sep=',', header=None, names=['x', 'y'])
    xlims = (data['x'].min() - 2, data['x'].max() + 2)
    ylims = (data['y'].min() - 2, data['y'].max() + 2)

    title = "N = 10000"
    outfile = "10000_2d.png"
    plt.clf()
    plt.title(title)
    plt.xlim(xlims[0], xlims[1])
    plt.ylim(ylims[0], ylims[1])
    plt.xlabel("X")
    plt.ylabel("Y")
    plt.scatter(data['x'], data['y'], s=0.5, c='g', alpha=0.5)
    plt.scatter(real['x'], real['y'], s=15, c='r', label="Centroids")
    plt.legend()
    plt.grid(linestyle='dashed', alpha=0.5)
    #plt.show()
    plt.savefig(outfile)
    return

if __name__ == "__main__":
    path = "2d/10000_samples_3_centers/"
    plot_everything(path)