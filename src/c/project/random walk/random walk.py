import numpy as np
import matplotlib.pyplot as plt

path="/Users/setsuna/Desktop/c4phy/src/c/project/random walk/rw.csv"
data = np.loadtxt(path,delimiter=",");

x=data.T[0]
y=data.T[1]

y1=4*np.sqrt(x)

fig, ax = plt.subplots(figsize=(18, 9))
ax.scatter(x, y, s=1, color='black', zorder=1)
ax.plot(x,y1,color='red', linewidth=1.2, zorder=2)
fig.suptitle("Random Walk Simulation")

plt.show()