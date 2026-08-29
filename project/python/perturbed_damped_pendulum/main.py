from pathlib import Path

import matplotlib.pyplot as plt
import numpy as np

base = Path(__file__).resolve().parents[2] / "c" / "perturbed_damped_pendulum"
theta_t = np.loadtxt(base / "theta-t.csv", delimiter=",", skiprows=1)
theta_omega = np.loadtxt(base / "theta-omega.csv", delimiter=",", skiprows=1)

fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(16, 8))
ax1.scatter(theta_t.T[0], theta_t.T[1], s=1, color="black", zorder=1)
ax1.set_title("t-theta")
ax2.scatter(theta_omega.T[0], theta_omega.T[1], s=1, color="black", zorder=1)
ax2.set_title("theta-omega")
fig.suptitle("damped driven pendulum")
plt.show()
