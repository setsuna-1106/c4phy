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

shooting algorithm

## 13.3 Classical Chaotic Scattering

One might expect that the classical scttering of a projectile from a passive target will vary smoothly. Yet experiments have found that when a projectile undergose multiple internal scatterings, its final trajectory appears unrelated to its initial one.

**Problem** Determine if multiple internal scatterings may lead to such a chaotic situation.

#### 13.3.1 Model and Theory

Our model for scattering from the bumpers in pinball machines is a point particle sscattering from the stationary 2D potential
$$
V(x,y)=x^2y^2e^{-(x^2+y^2)}
$$
The theeory for this problem is classical dynamics. Visualize a scattering experiment in which a projectile starts out at ($x=b,y=-\infin$) with velocity v. The distance b is called the *impact parameter*. After scattering and moving out to $y=+\infin$, the projectile is observed at the scattering angle $\theta$

Once we have that we can calculate the differerntial cross section:
$$
\sigma(\theta) = \left|\frac{d\theta}{db}\right| \frac{b}{\sin\theta(b)}
$$
As your computation should show, there are parameter values for which $d\theta/db$ gets very large, or even discontinuous, and this leads to chaotic cross sections.

#### 13.3.2 Implementation