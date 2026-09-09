from pathlib import Path

import numpy as np
import matplotlib.pyplot as plt

path = Path(__file__).resolve().parents[2] / "c" / "logistic_map" / "x.csv"
data = np.loadtxt(path, delimiter=",")

x = data.T[0]
y1 = data.T[1]
y2 = data.T[2]
y3 = data.T[3]

fig, ax = plt.subplots(figsize=(18, 9))
ax.plot(x[:20], y1[:20], color='black', linewidth=1, zorder=2)
ax.plot(x[:20], y2[:20], color='red', linewidth=1, zorder=2)
ax.plot(x[:20], y3[:20], color='blue', linewidth=1, zorder=2)
fig.suptitle("logistic map")

plt.show()