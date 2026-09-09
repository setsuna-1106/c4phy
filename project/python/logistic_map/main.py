from pathlib import Path

import numpy as np
import matplotlib.pyplot as plt

path = Path(__file__).resolve().parents[2] / "c" / "logistic_map" / "x.csv"
data = np.loadtxt(path, delimiter=",")

x = data.T[0]
y = data.T[1]


fig, ax = plt.subplots(figsize=(18, 9))
ax.plot(x[:100], y[:100], color='black', linewidth=1, zorder=2)
fig.suptitle("logistic map")

plt.show()