import matplotlib.pyplot as plt
from matplotlib import cm
import numpy as np

from mpl_toolkits.mplot3d import Axes3D
from matplotlib.ticker import LinearLocator, FormatStrFormatter

name_file = "names.txt"
# path = "../Perlin Noise/"
path = "Perlin Noise/"

names = []

try:
    name_stream = open(path + name_file, "r")
    for name in name_stream:
        names.append(name.rstrip())
    name_stream.close()
except FileNotFoundError:
    print(name_file + " was not found.")
    
for file in names:
    graph_stream = open(path + file)
    vals = graph_stream.readlines()
    graph_stream.close()

    Z = list(map(str.strip, vals))
    Z_list = [i.strip("[]").split(" ") for i in Z]
    Z_float = []
    for line in Z_list:
        Z_float.append([float(i) for i in line])
        pass
    fig = plt.figure()
    ax = fig.gca(projection='3d')

    X = np.arange(0, 2, 0.1).astype(np.float)
    Y = np.arange(0, 2, 0.1).astype(np.float)
    X, Y = np.meshgrid(X, Y)
    Z = np.array(Z_float).astype(np.float)
    Z = Z.reshape(20, 20)
    surf = ax.plot_surface(X, Y, Z, cmap=cm.coolwarm,
                           linewidth=0, antialiased=False)

    ax.set_zlim(0, 1.01)
    ax.zaxis.set_major_locator(LinearLocator(10))
    ax.zaxis.set_major_formatter(FormatStrFormatter('%.02f'))

    # Add a color bar which maps values to colors.
    fig.colorbar(surf, shrink=0.5, aspect=5)

    plt.show()
    pass
# name_file = "names.txt"
# config_file = "config.txt"
# path = "Perlin Noise/"
#
# names = []
# configs = []
#
# try:
#     name_stream = open(path + name_file, "r")
#     for name in name_stream:
#         names.append(name.rstrip())
#     name_stream.close()
# except FileNotFoundError:
#     print(name_file + " was not found.")
#
# try:
#     config_stream = open(path + config_file)
#     for config in config_stream:
#         configs.append(config)
#     config_stream.close()
# except FileNotFoundError:
#     print(config_file + "was not found.")
#
# time_step = configs[0]
# seconds = configs[1]
#
# x_vals = np.arange(0, int(seconds), float(time_step)).tolist()
#
# for graph in names:
#     graph_stream = open(path + graph)
#     y_vals = graph_stream.readlines()
#     y_vals = list(map(str.strip, y_vals))
#     fig, ax = plt.subplots()
#     ax.plot(np.asarray(x_vals, dtype=np.float32), np.asarray(y_vals, dtype=np.float32))
#     ax.set(title=graph)
#     ax.set_ylim([0, 1])
#
#
# plt.show()

