import numpy as np
from scipy.integrate import solve_ivp


m = 1.0
h = 1.0
a = 1.0


def dydt(
    x: float,
    y: np.ndarray,
    E: float
) -> np.ndarray:

    return np.array([
        y[1],
        -2 * m * E / h**2 * y[0]
    ])


def main() -> None:

    E = 0.0

    initial = [
        0.0,   # ψ(0)
        1.0,   # ψ'(0)
    ]

    x_span = (0.0, a)

    solution = solve_ivp(
        dydt,
        x_span,
        initial,
        t_eval=[a],
        args=(E,)
    )

    print(solution.y[0])


if __name__ == "__main__":
    main()
