from typing import Optional

import numpy as np
from scipy.integrate import solve_ivp
from scipy.optimize import brentq

m = 1.0
hbar = 1.0
a = 1.0


def dydt(
    x: float,
    y: np.ndarray,
    E: float
) -> np.ndarray:

    return np.array([
        y[1],
        -2 * m * E / hbar**2 * y[0]
    ])


def shoot(E: float, x_eval: Optional[np.ndarray] = None):
    solution = solve_ivp(
        dydt,
        (0.0, a),
        [0.0, 1.0],  # ψ(0)=0; the initial slope only sets the scale
        t_eval=x_eval,
        args=(E,),
        rtol=1e-10,
        atol=1e-12,
    )
    if not solution.success:
        raise RuntimeError(solution.message)
    return solution


def solve_state(n: int) -> tuple[float, np.ndarray, np.ndarray]:
    """Return energy, positions, and normalized wavefunction for state n."""
    if n < 1:
        raise ValueError("n must be a positive integer")

    energy_scale = np.pi**2 * hbar**2 / (2 * m * a**2)
    lower = energy_scale * (n - 0.5) ** 2
    upper = energy_scale * (n + 0.5) ** 2
    E = brentq(lambda energy: shoot(energy).y[0, -1], lower, upper)

    x = np.linspace(0.0, a, 501)
    psi = shoot(E, x).y[0]
    psi /= np.sqrt(np.trapezoid(psi**2, x))
    return E, x, psi


def main() -> None:
    for n in range(1, 4):
        E, _, _ = solve_state(n)
        print(f"n={n}: E={E:.10f}")

    print(f"ψ_n(x) = sqrt(2/{a:g}) sin(nπx/{a:g}) for 0 < x < {a:g}; 0 outside")


if __name__ == "__main__":
    main()
