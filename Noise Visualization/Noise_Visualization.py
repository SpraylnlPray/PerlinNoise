import matplotlib.pyplot as plt
import numpy as np

name_file = "names.txt"
config_file = "config.txt"
path = "Perlin Noise/"

names = []
configs = []

try:
    name_stream = open(path + name_file, "r")
    for name in name_stream:
        names.append(name.rstrip())
    name_stream.close()
except FileNotFoundError:
    print(name_file + " was not found.")

try:
    config_stream = open(path + config_file)
    for config in config_stream:
        configs.append(config)
    config_stream.close()
except FileNotFoundError:
    print(config_file + "was not found.")

time_step = configs[0]
seconds = configs[1]

x_vals = np.arange(0, int(seconds), float(time_step)).tolist()

for graph in names:
    graph_stream = open(path + graph)
    y_vals = graph_stream.readlines()
    y_vals = list(map(str.strip, y_vals))
    fig, ax = plt.subplots()
    ax.plot(np.asarray(x_vals, dtype=np.float32), np.asarray(y_vals, dtype=np.float32))
    ax.set(title=graph)
    ax.set_ylim([0, 1])


plt.show()

