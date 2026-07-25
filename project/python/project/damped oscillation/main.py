import matplotlib.pyplot as plt
import numpy as np

path = "/Users/setsuna/Desktop/c4phy/src/c/project/damped oscillation/x-t.csv"
data = np.loadtxt(path, delimiter=",")

t = data.T[0]
x = data.T[1]

fig, ax = plt.subplots(figsize=(9, 9))
ax.scatter(t, x, s=1, color="black", zorder=1)
fig.suptitle("t-x")

plt.show()
