## 8 Differential Equations and Nonlinear Oscillations

> *In this chapter we develop numerical methods for solving ordinry differential equations, and focus on applying those tools to nonlinear systems.*

### 8.1 Nonlinear Oscillators

Figure showsa a mass m attached to a spring that exerts a restoring force toword the orgin, as well as a hand that exerts a time-dependent external force on the mass. The restoring force exerted by the spring is nonlinear.

![截屏2026-06-01 21.05.21](/Users/setsuna/Desktop/c4phy/notes/Differential Equations and Nonlinear Oscillations/note.assets/截屏2026-06-01 21.05.21.png)

**Problem** Solve for the motion of the mass as a function of the time for an arbitrary restoring force. You may assume the motion is constrained to one dimension.

Using the Newton's second law provides us with the equation of motion
$$
F_k(x)+F_{ext}(x,t)=m\frac{d^2x}{dt^2}
$$
Where $F_k(x)$ is an arbitrary restoring force exerted by the spring and $F_{ext}(x,t)$ is the external force.

As our first model, we'll look at a potential that is linear for small displacements x, but becomes nonlinear for large x values:
$$
V(x) \simeq \frac{1}{2}kx^2
\left(1-\frac{2}{3}\alpha x\right),
$$

$$
\Rightarrow \quad
F_k(x)
=
-\frac{dV(x)}{dx}
=
-kx(1-\alpha x)
$$

$$
\Rightarrow \quad
m\frac{d^2x}{dt^2}
=
-kx(1-\alpha x).
$$

Where we have omitted the time-dependent external force. Equation (4) is the second-order ordinary differential equation (ODE) we need to solve. If $\alpha x<<1$, we should have essentially harmonic, but as $x\rightarrow1/\alpha$ the anharmonic effects should increase.

We can understand the basic physics of this model by looking at her curves on the left in Figure![截屏2026-06-01 23.52.42](/Users/setsuna/Desktop/c4phy/notes/Differential Equations and Nonlinear Oscillations/note.assets/截屏2026-06-01 23.52.42.png)

As long as $x<1/\alpha$, there will be a restoring force and the motion will be periodic (repeated exactly and indefintely in time), though it may not be harmonic.

As a second model of a nonlinear oscillator, we assume that the spring's potential function is proportional to some arbitrary even power p of x:
$$
V(x)=\frac1pkx^p,(p\ even)
$$
We require an even p to ensure that the force,
$$
F_k{x}=-\frac{dV(x)}{dx}=-kx^{p-1}
$$


### 8.2 ODE Review

#### 8.2.1 Order

A general form for a fist-order differential equation is
$$
\frac{dy}{dt}=f(t,y)
$$
where the "order" refers to the degree of the derivative on the LHS. The derivative or force function $f(t,y)$ on the RHS, in arbitrary.

A general form for a second-order differential equation is
$$
\frac{d^2y}{dt^2}+\lambda\frac{dy}{dt}=f(t,\frac{dy}{dt},y).
$$
The derivative function f on the RHS is arbitrary and may involve any power of the first derivative as well.

#### 8.2.2 Ordinary and Partial

Equations which contain only one independent variable are ODE. In contrast, an equation which contains more than one independent variables is a *partial differential equation* (PDE).

#### 8.2.3 Linear and Nonlinear

A *linear equation* is one in which only the first power of $y$ or $d^ny/d^nt$ appears; a *nonlinear equation* may contain higher power.

An important property of linear equations is the *law of linear superposition* that lets us add different solutions together to form new ones.

#### 8.2.4 Initial and Boundary Condition

The general solution of a first-order differential equation contains one arbitrary constant. The general solution of a second-order differential equation contains two such constants, and so forth.

For any specific problem, these constants are usually determined by the *initial conditions*. You must know the initial conditions in order to obtain a unique solution to differntial equation.

In addition to the initial conditions, it is possible to further restrict the solutions of differential equations. One such way is by boundary conditions that constrain the solution to have fixed values at the boundaries of the solution space.

### 8.3 Dynamic Form of ODEs

A standard forms for ODEs is to express ODEs of *any order* as N simultaneous first-order ODEs in the N unknowns, $y^i,i=0,N-1$:
$$
\begin{aligned}
\frac{dy^{(0)}}{dt}
&=
f^{(0)}\left(t,\{y^{(i)}\}\right),\\
\frac{dy^{(1)}}{dt}
&=
f^{(1)}\left(t,\{y^{(i)}\}\right),\\
&\vdots\\
\frac{dy^{(N-1)}}{dt}
&=
f^{(N-1)}\left(t,\{y^{(i)}\}\right).
\end{aligned}
$$
Note,$f$ can contain an explicit dependence on any or all of the $y^{(i)}$, but not explicitly on a devative $dy^{(i)}/dt$. These equations can be expressed more succinctly by use of the N-dimensional vectors **y** and **f**:
$$
\frac{d\mathbf{y}(t)}{dt}=\mathbf{f}(t,\mathbf{y}),
$$

$$
\mathbf{y}
=
\begin{bmatrix}
y^{(0)}(t)\\
y^{(1)}(t)\\
\vdots\\
y^{(N-1)}(t)
\end{bmatrix},
\qquad
\mathbf{f}
=
\begin{bmatrix}
f^{(0)}(t,\mathbf{y})\\
f^{(1)}(t,\mathbf{y})\\
\vdots\\
f^{(N-1)}(t,\mathbf{y})
\end{bmatrix}.
$$

The utility of such compact notation is that we can study the properties of the ODEs, as well as develop algorithms to solve them. To see how this works in partice, let's convert Newton's law
$$
\frac {d^2x}{dt^2}=\frac1mF(t,x,\frac{dx}{dt})
$$
to this standard form.

### 8.4 ODE Algorithms

The classic way to solve an ODE is shown in Figure. One starts with the known initial value of the dependent variable, $y_0=0(t=0)$, and then uses the derivative function $f(t,y)$ to advanc the initial value one small step $h$ forward in time to produce $y(t=h)=y_1$. Once you can do that, you can solve the ODE for all t values by just continuing to step to larger times, one small h at a time.![截屏2026-06-03 17.50.34](/Users/setsuna/Desktop/c4phy/notes/Differential Equations and Nonlinear Oscillations/note.assets/截屏2026-06-03 17.50.34.png)

Error is always a concern when integrating differential equations because derivatives require small differences, and small differences are prone to subtractive cancellations and round-off error accumulation. 

#### 8.4.1 Euler's Rule

Euler's rule is the simpleast algorithmfor integrating the differential equation by one step. It is just an application of the forward-difference algorithm for the derivative:
$$
\frac{d\mathbf{y}(t)}{dt}
\simeq
\frac{\mathbf{y}(t_{n+1})-\mathbf{y}(t_n)}{h}
=
\mathbf{f}(t,\mathbf{y}),
$$

$$
\Rightarrow \quad
\mathbf{y}_{n+1}
\simeq
\mathbf{y}_n
+
h\mathbf{f}(t_n,\mathbf{y}_n).
$$

We know from our discussion of differentiation that the error in the forward-difference algorithm is $𝒪(h^2)$, and so then this too is the error in Euler’s rule.

![截屏2026-06-03 18.00.36](/Users/setsuna/Desktop/c4phy/notes/Differential Equations and Nonlinear Oscillations/note.assets/截屏2026-06-03 18.00.36.png)

To indicate the aimplicity of this algorithm, we apply it to our oscillator problem for the first time step:
$$
y_1^{(0)}=x_0+v_0h,\quad y_1^{(1)}=v_0+h\frac1m[F_{ext}(t=0)+F_k(t=0)].
$$
Whereas we do not recommend Euler’s algorithm for general use, it is commonly used to start off more precise algorithms.

#### 8.4.2 Runge-Kutta Rule

The Runge-Kutta algorithm for integrating a differential equation is based upon the formal (exact) integral of our differential equation:
$$
\frac{dy}{dt} = f(t, y) \quad \Rightarrow \quad
y(t) = \int f(t, y)\, dt
$$

$$
\Rightarrow \quad
y_{n+1} = y_n + \int_{t_n}^{t_{n+1}} f(t, y)\, dt
$$

To derive the second-order Runge-Kutta algorithm **rk2**, we expand $f(t,y)$ in a Taylor series about the midpoint of the integration interval and retain two terms in the expansion:
$$
f(t, y) \simeq 
f(t_{n+1/2}, y_{n+1/2}) 
+ (t - t_{n+1/2}) \frac{df}{dt}(t_{n+1/2}) 
+ \mathcal{O}(h^2)
$$
Since $(t-t_{n+1/2})$ raised to any odd power is equally positive and negative over the interval $[t_n,t_{n+1}]$, the integral of the $(t-t_{n+1/2})$ term in vanishes and we obtain the **rk2 algorithm**:
$$
\int_{t_n}^{t_{n+1}} f(t, y)\, dt 
\simeq f(t_{n+1/2}, y_{n+1/2}) h + \mathcal{O}(h^3)
$$

$$
\Rightarrow \quad
y_{n+1} \simeq y_n + h f(t_{n+1/2}, y_{n+1/2}) + \mathcal{O}(h^3)
$$

And there's the rub, for we do not know the value of $y_{n+1/2}$ and cannot use this algorithm to determine it. The way out of this quandary is to use Euler's algorithm to determine $y_{n+1/2}$:
$$
y_{n+1/2} \simeq y_n + \frac{1}{2} h \frac{dy}{dt} 
= y_n + \frac{1}{2} h f(t_n, y_n)
$$
Putting the pieces all together gives the complete **rk2** algorithm:
$$
\mathbf{y}_{n+1} \simeq \mathbf{y}_n + \mathbf{k}_2, \quad (\text{rk2})
$$

$$
\mathbf{k}_1 = h \mathbf{f}(t_n, \mathbf{y}_n), \quad
\mathbf{k}_2 = h \mathbf{f}\Big(t_n + \frac{h}{2}, \mathbf{y}_n + \frac{\mathbf{k}_1}{2}\Big)
$$

Where we use boldface to indicate the vector nature of y and f. We see that the known derivative function F is evaluated at the ends and the midpoint of the interval, but only the initial value of the dependent variable y is required. This makes the algorirhm self-starting.

The fourth-order Runge-Kutta method **rk4** obtains $\mathcal O(h^4)$ precision by approximating y as a Talor series up to order $h^2$ (a parabola) at the midpoint of the interval, which again leads to cancellation of lower-order error.All in all, rk4 provides an excellent balance of power, precision, and programming simplicity. With rk4 there are four intermediate slopes, and these are approximated with the Euler algorithm:
$$
\mathbf{y}_{n+1}
=
\mathbf{y}_n
+
\frac{1}{6}
\left(
\mathbf{k}_1
+
2\mathbf{k}_2
+
2\mathbf{k}_3
+
\mathbf{k}_4
\right),
$$

$$
\mathbf{k}_1
=
h\mathbf{f}(t_n,\mathbf{y}_n),
$$

$$
\mathbf{k}_2
=
h\mathbf{f}
\left(
t_n+\frac{h}{2},
\mathbf{y}_n+\frac{\mathbf{k}_1}{2}
\right),
$$

$$
\mathbf{k}_3
=
h\mathbf{f}
\left(
t_n+\frac{h}{2},
\mathbf{y}_n+\frac{\mathbf{k}_2}{2}
\right),
$$

$$
\mathbf{k}_4
=
h\mathbf{f}
\left(
t_n+h,
\mathbf{y}_n+\mathbf{k}_3
\right).
$$



#### 8.4.3 Adams-Bashful-Moulton Predictor-Corrector Rule

Another approach for obtaining high precision in an ODE algorithm uses the solution from twi previous steps, $y_{n-2}$ and $y_{n-1}$, in addition to $y_n$, to predict $y_{n+1}$



### 8.5 Solution for Nonlinear Oscillations

