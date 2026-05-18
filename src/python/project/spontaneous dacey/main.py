import numpy as np
import matplotlib.pyplot as plt

path="/Users/setsuna/Desktop/c4phy/src/c/project/spontaneous Decay/number_of_particles.csv"
data = np.loadtxt(path,delimiter=",");

x=data.T[0]
y=data.T[1]

y1=np.log10(y)

fig, ax = plt.subplots(figsize=(6, 4))
plt.plot(x, y1)

fig.suptitle("sponaneous dacey")
plt.xlabel("t")
plt.ylabel("logN")

plt.show()