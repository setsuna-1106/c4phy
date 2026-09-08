from pathlib import Path

import matplotlib.pyplot as plt
import numpy as np

path = Path(__file__).resolve().parents[2] / "c" / "classical_chaotic_scattering" / "b-theta.csv"
data = np.loadtxt(path, delimiter=",")

b = data.T[0]
theta = data.T[1]

fig, ax = plt.subplots(figsize=(9, 4))
ax.scatter(b, theta, s=1, color="black", zorder=1)
fig.suptitle("b-theta")

plt.show()
