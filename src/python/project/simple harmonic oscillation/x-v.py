import matplotlib.pyplot as plt
import numpy as np

path1 = (
    "/Users/setsuna/Desktop/c4phy/src/c/project/simple harmonic oscillations/x-v1.csv"
)
path2 = (
    "/Users/setsuna/Desktop/c4phy/src/c/project/simple harmonic oscillations/x-v2.csv"
)
data1 = np.loadtxt(path1, delimiter=",")
data2 = np.loadtxt(path2, delimiter=",")

x = np.arange(1, 1001)
x1 = data1.T[0]
v1 = data1.T[1]
E1 = data1.T[2]

x2 = data2.T[0]
v2 = data2.T[1]
E2 = data2.T[2]

fig, axes = plt.subplots(2, 1, figsize=(9, 9))
axes[0].scatter(x1, v1, s=1, color="green", zorder=1)
axes[0].scatter(x2, v2, s=1, color="blue", zorder=1)
fig.suptitle("x-v")

axes[1].plot(x, E1)
axes[1].plot(x, E2)

plt.show()
