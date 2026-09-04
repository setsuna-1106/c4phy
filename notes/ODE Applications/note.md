# ODE Applications; Eigenvalues, Scattering, Trajectories

## 13.1 Quantum Eigenvalues for Arbitrary Potentials

**Problem** What is the energy of a particle bound by a potential that confines it to an atomic distance

If a particle of energy E is moving in one dimension and experiences a potential $V(x)$, that wave function is determined by an ordinary differential equation, the time-independent Schrödinger equation:
$$
\frac{-\hbar^2}{2m} \frac{d^2\psi(x)}{dx^2} + V(x)\psi(x) = E\psi(x)
$$
In practive, we solve for the wave vector $\kappa$, where it is related to bound states ($E<0$) by:
$$
\kappa^2 = -\frac{2m}{\hbar^2}E.
$$
The Schrödinger equation now takes the form
$$
\frac{d^2\psi(x)}{dx^2} - \frac{2m}{\hbar^2}V(x)\psi(x) = \kappa^2\psi(x).
$$

### 13.1.1 Model: Nucleon in a Box

$$
V(x) = \begin{cases} 
-V_0 = -83\,\text{MeV}, & \text{for } |x| \leq a = 2\,\text{fm}, \\
0, & \text{for } |x| > a = 2\,\text{fm},
\end{cases}
$$

where values of 83 MeV for the depth, and 2 fm for the radius, are typical for nuclear-bound states. With this potential the Schrödinger equation becomes
$$
\frac{d^2\psi(x)}{dx^2} + \left(\frac{2m}{\hbar^2}V_0 - \kappa^2\right)\psi(x) = 0, \quad \text{for } |x| \leq a,
$$

$$
\frac{d^2\psi(x)}{dx^2} - \kappa^2\psi(x) = 0, \quad \text{for } |x| > a.
$$

## 13.2 Algorithm: ODE Solver + Search

