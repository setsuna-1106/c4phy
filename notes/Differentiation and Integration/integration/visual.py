import os

import matplotlib.pyplot as plt
import numpy as np

path = os.path.join(os.path.dirname(__file__), "error.csv")
data = np.genfromtxt(path, delimiter=",", invalid_raise=False)

x = data.T[0]
y1 = data.T[1]
# y2 = data.T[2]

fig, ax = plt.subplots(figsize=(6, 4))
plt.loglog(
    x, np.abs(y1), "o-", markersize=2, color="red", label="trapezoid  O(1/N$^2$)"
)
# plt.loglog(
# x, np.abs(y2), "s-", markersize=2, color="green", label="Simpson     O(1/N$^4$)"
# )


plt.grid(True, alpha=0.3)
plt.legend()
plt.xlabel("N")
plt.ylabel("absolute error")
plt.title("Numerical integration of sin(x) on [0, 1]")
plt.show()
