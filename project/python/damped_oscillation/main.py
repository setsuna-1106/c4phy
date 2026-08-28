from pathlib import Path

import matplotlib.pyplot as plt
import numpy as np

path = Path(__file__).resolve().parents[2] / "c" / "damped_oscillation" / "x-t.csv"
data = np.loadtxt(path, delimiter=",")

t = data.T[0]
x = data.T[1]

fig, ax = plt.subplots(figsize=(9, 9))
ax.scatter(t, x, s=1, color="black", zorder=1)
fig.suptitle("t-x")

plt.show()
