import numpy as np
import pandas as pd
from matplotlib import pyplot as plt
import os

def plot_everything(path, path_save_img):
    data_path = os.path.join(path, "data.csv")
    centr_path = os.path.join(path, "centroids.csv")
    ms_path = os.path.join(path, "convergence")

    data = pd.read_csv(data_path, sep=',', header=None, names=['x', 'y'])
    real = pd.read_csv(centr_path, sep=',', header=None, names=['x', 'y'])
    xlims = (data['x'].min() - 2, data['x'].max() + 2)
    ylims = (data['y'].min() - 2, data['y'].max() + 2)

    k = lambda name : float(name.split("conv_")[-1].split('.')[0])
    csv = sorted([os.path.join(ms_path, f) for f in os.listdir(ms_path) if f.endswith(".csv")], key=k)
    
    for c in csv:
        name = c.split('/')[-1]
        outfile = f"{name.split('.')[0]}.png"
        centroids = pd.read_csv(c, sep=',', header=None, names=['x', 'y'])
        plt.clf()
        plt.title(name)
        plt.xlim(xlims[0], xlims[1])
        plt.ylim(ylims[0], ylims[1])
        plt.scatter(data['x'], data['y'], s=0.5, c='g', alpha=0.5)
        plt.scatter(real['x'], real['y'], s=15, c='r', label="Real")
        plt.scatter(centroids['x'], centroids['y'], s=15, c='b', label="Computed")
        plt.legend()
        plt.show()
        #plt.savefig(os.path.join(path_save_img, outfile))
    return

if __name__ == "__main__":
    path = "2d/100000_samples_3_centers/"
    path_save_img = os.path.join(path, "convergence", "img")
    if not os.path.exists(path_save_img):
        os.mkdir(path_save_img)
    plot_everything(path, path_save_img)