# 15 Nonlinear Population Dynamics

We view nonlinear dynamics as one of the success stories of computational physics.

## 15.1 The Logistic Map, A Bug Population Model

**Problem** Deduce if a simple law can produce such complicated behaviors

We want to develop a model of how $N_n$ varies with the generation number n.

We start our modeling by reversing the sign of $\lambda$ in the discrete decay law, which should give us *growth*:
$$
\frac {\Delta N_i}{\Delta t}=\lambda N_i
$$
Yet we know that exponential growth eventually tapers off, with the population reaching a maximum $N_*$ ( the carrying capacity ). Consequently, we modify the growth model by changing the growth rate parameter $\lambda$ to one that decreases as the population approaches $N_*$:
$$
\lambda=\lambda'(N_*-N_i)
$$

$$
\frac {\Delta N_i}{\Delta t}=\lambda'(N_*-N_i)N_i
$$

It is usually written in a form that relates the number of bugs in the future of the number in the present generation:
$$
\begin{align}
N_{i+1} &= N_i + \lambda' \Delta t (N_* - N_i) N_i, \\
&= N_i \left( 1 + \lambda' \Delta t N_* \right) \left[ 1 - \frac{\lambda' \Delta t}{1 + \lambda' \Delta t N_*} N_i \right].
\end{align}
$$
This relation looks simpler when expressed in terms of dimensionless variables:
$$
\begin{align}
x_{i+1} &= \mu x_i (1 - x_i), \\
\mu &\stackrel{\text{def}}{=} 1 + \lambda' \Delta t N_*, \\
x_i &\stackrel{\text{def}}{=} \frac{\lambda' \Delta t}{1 + \lambda' \Delta t N_*} N_i \simeq \frac{N_i}{N_*}.
\end{align}
$$
Here $x_i$ is a dimensionless population variable and $\mu$ is a dimensionless growth parameter.

The map is seen to be the sum of linear and quadratic dependencies on $x_i$. It is called a map because it converts one number in a sequence to the next,
$$
x_{i+1}=f(x_i)
$$
For the logistic map, $f(x)=\mu x(1-x)$, with the quadratic dependence on x making this a nonlinear map, and the dependence on only the one variable x making it a one-dimensional map.



### 15.1.1 Exploring Map Properties

