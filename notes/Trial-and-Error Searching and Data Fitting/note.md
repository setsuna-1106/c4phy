## Trial-and-Error Searching and Data Fitting

> This chapter adds some more tools to our computatianal toolbox.First, we examine ways to solve equations via a trial-and-error search.Second, we examine the fitting of curves to data. 



### 6.1 Quantum Bound States I

Many computational techniques use well-defined algorithm leading to definite outcomes. In contrast, some techniques use trial-and-error algorithm in which internal decisions are made as to what steps to follow, and in which a number of solutions may be tried before one is settled upon, or not. 
Writing this type of program is usually challenging because we must foresee a number of possible outcomes, with the chance of failure always present.

Probably the most standard problem in quantum mechanics, is to solve foe the energies of a particle of mass $m$ bounf within a 1D square well of radius a:
$$
V(x)=
\begin{cases}
-V_0, & \text{for } |x|\le a, \\
0, & \text{for } |x|\ge a.
\end{cases}
$$
 As shown in quantum mechanics texts, the energies of the bound states $E=-E_B <0$ within this well are solutions of the transcendental equations
$$
\sqrt{10-E_B}\,
\tan\!\left(\sqrt{10-E_B}\right)
=
\sqrt{E_B},
\qquad (\text{even}),
$$

$$
\sqrt{10-E_B}\,
\cot\!\left(\sqrt{10-E_B}\right)
=
\sqrt{E_B},
\qquad (\text{odd}).
$$

Where even and odd refer to the symmetry of the wave function. Here we have chosen units such that $\hbar = 1,\quad 2m = 1,\quad a = 1,\quad \text{and}\quad V_0 = 10.$

The problem is to 

1. Find several bound-state energies $E_B$ for even wave functions.
2. Explore how making the potential deeper, say, by changing the 10 to a 20 or a 30, affects the number of states and their energies.



### 6.2 Bisection Search

Trial-and-error root finding looks for a value of x for which
$$
f(x)\simeq0,
$$
where we follow the convention of moving what might, otherwise, be on the right-hand-side(RHS) of an equation to the left-hand-side (LHS) in order to leave just a 0 on the RHS.

The search procedure starts with a guessed value for x, substitudes that guess into $f(s)$(the "trial"), and then sees how different the LHS is from zero (the "error").The algorithm then changes x based on the error, and tris out the new guess in $f(x)$. The procedure continues until $f(x)\simeq0$ to some desired level of precision,or until the changes ins x are insignificant, or when the search seems endlss.

The most elementary trial-and-error technique is the *bisection algorithm*.Is is reliable but slow.If you know some intervak in which $f(x)$ changes sign,then bisection algorithm will always converge to the root by finding progressively smaller and smaller intervals within which the zero lies.

The basis of the bisection algorithm is shown in Figure.![截屏2026-05-27 16.31.01](/Users/setsuna/Desktop/c4phy/notes/Trial-and-Error Searching and Data Fitting/note.assets/截屏2026-05-27 16.31.01.png)



### 6.3 Newton-Raphson Search

The Newton-Raphson algorithm can find roots of the $f(x)=0$ more quickly than the bisection method.

![截屏2026-05-28 18.47.56](/Users/setsuna/Desktop/c4phy/notes/Trial-and-Error Searching and Data Fitting/note.assets/截屏2026-05-28 18.47.56.png)

If a guess is in a region where $f(x)$ is nearly linear, then the convergence is very rapid.

The analytic formulation of the Newton-Raphson algorithm starts with an old guess $x_0$, and expresses a new guess x as the old guess plus a correction $\Delta x$:
$$
x_0 = \text{old guess},
\qquad
\Delta x = \text{unknown correction}
$$

$$
\Rightarrow\quad
x = x_0 + \Delta x.
$$

We next expand the known function $f(x)$ in a Taylor series arond $x_0$, and keep only the linear term in the expansion:
$$
f(x=x_0+\Delta x)
\simeq
f(x_0)
+
\left.
\frac{df}{dx}
\right|_{x_0}
\Delta x.
$$
We determine the correction $\Delta x$ by calculating the point at which this linear approximation to $f(x)$ crosses the x-axis:
$$
f(x_0)
+
\left.
\frac{df}{dx}
\right|_{x_0}
\Delta x
=
0,
$$

$$
\Rightarrow\quad
\Delta x
=
-
\frac{
f(x_0)
}{
\left.
\dfrac{df}{dx}
\right|_{x_0}
}.
$$

The procedure is repeated, starting at the improved x, until some set level of precision is obtianed.

But sometime the Newton-Raphson search will fail.

![截屏2026-05-28 18.59.26](/Users/setsuna/Desktop/c4phy/notes/Trial-and-Error Searching and Data Fitting/note.assets/截屏2026-05-28 18.59.26.png)

#### 6.3.1 Search + Backtracking

Two examples of posiible problems with the Newton-Raphson algorithm are shown in Firgure.On the left, we can see a case where teh search takes us to an x value where teh functino has a local extremum(minimum or maximum),that is, where $df/dx=0$.

When it happens, you need to start your search with a different guess.

The other problem is that a search falls into an infinite loop surronding the zero,without ever getting there.

A solution to this problem is *backtracking*.As the name implies, in cases where the new guess $x_0+\Delta x$leads to an increase in the magnitude of the function,$|f(x_0+\Delta x)|^2>|f(x_0)|^2$, you can backtrack somewhat and  try a smaller guess, say, $x_0+\Delta x/2$.If the magnitude of f still increase, then you just need to backtrack some more.

The problem in both these cases is that the initial guesses were not close enough to the regions where $f(x)$ is approximately linear.



### 6.4 Magnetization Search

**Problem** Determine $M(T)$ the magnetization as a function of temperature for simple magnetic materials.



### 6.5 Data Fitting

**Problem** The cross section measured for the resonant scattering of neutrons from a nucleus are given in Table. Your problem is to determine values for the cross sections at energy values lying between those in the table.

![截屏2026-05-29 15.09.31](/Users/setsuna/Desktop/c4phy/notes/Trial-and-Error Searching and Data Fitting/note.assets/截屏2026-05-29 15.09.31.png)The simplest is to numerically interpolate bewteen the values of the experimental $f(E_i)$ given in Table.This is direct and easy, but dose not account for there being experimental noise in the data.
A more appropriate solution is to find the *best fit* of a theoretical function to the data.

We start with what we believe to be the "correct" theoretical description of the data,
$$
f(E)
=
\frac{f_r}
{(E-E_r)^2+\Gamma^2/4}.
$$
Where $f_r,E_r,and\ \Gamma$ are unknown parameters. We then adjust the parameters to obtain the best fit. This is a best fit in a statistical sense, but in fact may not pass through all (or any) of the data points.

These two techniques of interpolaion and least-squares fitting are powerful tools that let you treat tables of numbers as if they were analytic functions, and sometimes let you deduce statistically meaningful constants or conclusion from measurements.

Consider Table as ordered data.We assume that $g(x)$ can be approximated as an (n-1)th-degree polynomial in each interval i:
$$
g_i(x)\simeq a_0+a_1x+a_2x^2+...+a_{n-1}x^{n-1}.
$$
Seeing that our fit is local, we do not assume that one $g(x)$ can fit all the data in the table, but instead use a diffenrent polynomial, that is, a different set of $a_i$ values, for each interval.

The classic interpolation formula was created by Lagrange. He figured out a closed-form expression that directly fits the (n-1) order polynomial to n values of the funtion $g(x)$ evaluated at the points $x_i$. The formula for each interval is written as the sum of polynomials:
$$
g(x) \simeq g_1\lambda_1(x)+g_2\lambda_2(x)+\cdots+g_n\lambda_n(x),
$$

$$
\lambda_i(x)=
\prod_{\substack{j=1\\ j\ne i}}^{n}
\frac{x-x_j}{x_i-x_j}
=
\frac{x-x_1}{x_i-x_1}
\frac{x-x_2}{x_i-x_2}
\cdots
\frac{x-x_n}{x_i-x_n}.
$$

If the data contain little noise, this polynomial can be used with some confidence within the range of the data, but with risk beyond the range of the data.

Notice that Lagrange interpolation makes no restriction that the points xi be evenly spaced. Usually, the Lagrange fit is made to only a small region of the table with a small value of n, despite the fact that the formula works perfectly well for fitting a high-degree polynomial to the entire table. The difference between the value of the polynomial evaluated at some x and that of the actual function can be shown to be the remainder
$$
R_n(x) = \frac{(x - x_1)(x - x_2)\cdots(x - x_n)}{n!}\, g^{(n)}(\xi),
\quad \xi \in (x_1, x_n)
$$
where $\xi$ lies somewhere in the interpolation interval.

#### 6.5.2 Cubic Spline Interpolation

A siphisticated variation of an n=4 interpolation, known as *cubic splines*, often leads to surprisingly smooth and eye-pleasing fits.

The series of cubic polynomials obtained by spline-fitting a table of data can be integrated and differentiated, and is guaranteed to have well-behaved derivatives.

The basic approximation of splines is the representation of the function $g(x)$ in the subinterval $[x_i,x_{i+1}]$ with a cubic polynomial:
$$
g(x) \approx g_i(x), \quad x_i \le x \le x_{i+1}
$$

$$
g_i(x) = g(x_i) + g'(x_i)(x - x_i) 
+ \frac{1}{2} g''(x_i)(x - x_i)^2 
+ \frac{1}{6} g'''(x_i)(x - x_i)^3
$$

This representation makes it clear that the coefficients in the polynomial equal the values of $g(x)$ and its first, second, and third derivatives at the tabulated points $x_i$.Derivatives beyond the third vanish for a cubic.
The matching of $g_i$ at the *nodes* that connect one interval to the next provides the equations
$$
g_i(x_{i+1})=g_{i+1}(x_{i+1}),i=1,N-1.
$$
The matching of the first and second derivatives at each interval's boundaries provides the equations
$$
g'_{i-1}(x_i) = g'_i(x_i), \quad 
g''_{i-1}(x_i) = g''_i(x_i)
$$
The additional equations needed to determine all constants are obtiained by matching the third derivatives at adjacent nodes.Values for the third derivatives are found by approximating them in terms of the second derivatives:
$$
g'''_i\simeq\frac{g''_{i+1}-g''_i}{x_{i+1}-x_i}.
$$
We can see that matching at the boundaries of the intervals results in only (N-2) linear equations for N unknowns. It usually is taken to be the boundary conditions at the endpoints $a=x_1$ and $b=x_N$, specifically, the second dervatives there $g''(a)$ and $g''(b)$. There are several ways to determine these second derivatives:

1. **Natural spline**:Set $g''(a)=g''(b)=0$; that is, permit the function to have a slope at the endpoints but no curvature. This is "natural" because the derivative vanishes for the flexible spline drafting tool.
2. **Input values for $g'$ or $g''$ at the boundaries**



### 6.6 Fitting Exponential Decay

The basic law of nature for spontaneous decay is that the number of decays $\Delta N$ in a time interval $\Delta t$ is proportional to the number of particles $N(t)$ present at that time and to the time interval
$$
\Delta N(t)
=
-\frac{1}{\tau}N(t)\Delta t
\quad\Rightarrow\quad
\frac{\Delta N(t)}{\Delta t}
=
-\lambda N(t).
$$
Here $\tau=1/\lambda$ is the *lifetime* of the particle, with $\lambda$ a rate parameter. The actual decay *rate* is given by the second equation



### 6.7 Least-Squares Fitting

What is meant by a "good fit" to experimental data. However, we will emphasize three points:

1. If the data being fit contain errors, then the "best fit" in a statistical sense should not pass through all the data points.
2. If the theory is not an appropriate one for the data, then its best fit to the data may not be a good fit at all. This is good, for this is how we know that the theory is not appropriate.
3. Only for the simplest case of a linear least-squares fit can we write down a closed-form solution to evaluate and obtain the best fit.

Imagine that you have measured $N_D$ data values of the independent variable y as a function of the dependent variable x:
$$
(x_i, y_i \pm \sigma_i),\quad i = 1, N_D
$$
where $\pm \sigma_i$ is the experimental uncertianty in the ith value of y. (For simplicity we assume that all the errors $\sigma_i$ Occur in the dependent variable, although this is hardly ever true). For our problem, y is the number of decays as function of time , and $x_i$ is the times. Our goal is to determine how well a mathematical function $y=g(x)$ can describe these data.
Additionally, if the theory contains some parameters or constants, our goal is also to determine the best values for these parameters.

We assume that the theory funcion $g(x)$ contain, in additiona to the functional dapendence on x, an additional dependence upon $M_P$ parameters ${a_1,a_2,...,a_{M_P}}$. Notice that the parameter $\{a_m\}$ are not variables but rather are parts of the theoretical model.

We indicate this as 
$$
g(x)=g(x;\{a_1,a_2,\ldots,a_{M_P}\})=g(x;\{a_m\})
$$
Where the $a_i$'s are parameters and x the independent variable.

We use the chi-square, $\chi^2$, measure as a gague of how well a theoretical function g reproduces data:
$$
\chi^2 \overset{\mathrm{def}}{=} 
\sum_{i=1}^{N_D}
\left(
\frac{y_i - g(x_i;\{a_m\})}{\sigma_i}
\right)^2
$$
where the sum is over the $N_D$ experimental points. The definition is such that smaller values of $\chi^2$ are better fits, with $\chi^2=0$ occuring if the theoretical curve went through the center of every data point. Notice also that the $1/\sigma_i^2$ weighting means that measurements with larger errors contribute less to $\chi^2$.

Least-squares fitting refers to adjustong the parameters in the theory until a minimum in $\chi^2$ is found, that is, finding a curve that produces the least value for the summed squares of the deviations of the data from the function $g(x)$.

The $M_P$ prameters that make $\chi^2$ an extremum are found by solving the $M_P$ equations:
$$
\frac{\partial \chi^2}{\partial a_m}=0,
\quad \Rightarrow \quad
\sum_{i=1}^{N_D}
\frac{\left[y_i-g(x_i)\right]}{\sigma_i^2}
\frac{\partial g(x_i)}{\partial a_m}
=0,
\quad (m=1,M_P).
$$


### 6.8 Nonlinear Fit to a Resonance

