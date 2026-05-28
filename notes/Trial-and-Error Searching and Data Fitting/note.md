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