import numpy as np
import matplotlib.pyplot as plt

# from scipy.optimize import curve_fit

# # ── load data ────────────────────────────────────────────────
# data = np.loadtxt(
#     "/Users/setsuna/Desktop/c4phy/src/c/project/spontaneous Decay/number_of_particles.csv",
#     delimiter=",",
# )
# t = data[:, 0]
# N = data[:, 1]

# # ── exponential fit:  N(t) = N0 * exp(-λ t) ──────────────────
# def model(t, N0, lam):
#     return N0 * np.exp(-lam * t)

# popt, pcov = curve_fit(model, t, N, p0=[N[0], 0.1])
# N0_fit, lam = popt
# N0_err, lam_err = np.sqrt(np.diag(pcov))
# half_life = np.log(2) / lam
# N_fit = model(t, *popt)
# residuals = N - N_fit

# # ── plot ─────────────────────────────────────────────────────
# fig, (ax1, ax2) = plt.subplots(
#     2, 1, figsize=(8, 7),
#     gridspec_kw={"height_ratios": [3, 1]},
#     sharex=True,
# )
# fig.suptitle("Spontaneous Decay  —  Monte Carlo Simulation", fontweight="bold")

# ax1.semilogy(t, N, "o", markersize=2, color="C0", label="MC data")
# ax1.semilogy(t, N_fit, "-",  color="C3", label=f"fit:  $N_0 e^{{-\lambda t}}$\n$N_0 = {N0_fit:.1f}$\n$\lambda = {lam:.6f}$")
# ax1.set_ylabel("N(t)")
# ax1.legend()
# ax1.grid(True, alpha=0.3)

# ax2.plot(t, residuals, ".", markersize=1.5, color="C0")
# ax2.axhline(0, color="C3", linewidth=0.8)
# ax2.set_xlabel("time step  $t$")
# ax2.set_ylabel("residuals")

# info = (
#     f"$T_{{1/2}} = {half_life:.2f}$ steps        "
#     f"$N_0 = {N0_fit:.0f} \\pm {N0_err:.0f}$        "
#     f"$\\lambda = {lam:.6f} \\pm {lam_err:.6f}$"
# )
# fig.text(0.5, 0.01, info, ha="center", fontsize=9)

# plt.tight_layout(rect=(0, 0.04, 1, 0.95))
# plt.savefig("spontaneous_decay.pdf")
# plt.savefig("spontaneous_decay.png", dpi=150)


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
