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