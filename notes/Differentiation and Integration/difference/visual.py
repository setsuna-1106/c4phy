import matplotlib.pyplot as plt
import numpy as np

import os
path = os.path.join(os.path.dirname(__file__), "error.csv")
data = np.loadtxt(path, delimiter=",")

x = data.T[0]
y1 = data.T[1]
y2 = data.T[2]
y3 = data.T[3]

fig, ax = plt.subplots(figsize=(6, 4))
plt.loglog(x, np.abs(y1), "o-", color="red", label="forward  O(h)")
plt.loglog(x, np.abs(y2), "s-", color="green", label="central  O(h²)")
plt.loglog(x, np.abs(y3), "D-", color="blue", label="extrapolated  O(h⁴)")
plt.grid(True, alpha=0.3)
plt.legend()
plt.xlabel("h")
plt.ylabel("absolute error")
plt.title("Numerical differentiation of sin(x) at x=1")

plt.show()
