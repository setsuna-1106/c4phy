## Matrix Computing and N-D Searching

> This chapter discusses how to compute with matrices, and, in particular, the use of the Python matrix and linear algebra packages. The chapter ends with a discussion of how to speed up large matrix computations.

### 7.1 Masses on a String and N-D Searching

**Problem** Two masses with weights $(W_1,W_2)=(10,20)$ are connected by three pieces of sting with lengths $(L_1,L_2,L_3)=(3,4,4)$, and hung from a horizontal bar of length $L=8$.Find the angles assumed by the strings and the tensions exerted by the strings.![截屏2026-05-30 15.08.06](/Users/setsuna/Desktop/c4phy/notes/Matrix Computing and N-D searching/Matrix Computing and N-D searching.assets/截屏2026-05-30 15.08.06.png)

We start with the geometric constraints that the horizontal length of the structure is L, and that the strings begin and end at the same height
$$
\begin{gathered}
L_1 \cos \theta_1 + L_2 \cos \theta_2 + L_3 \cos \theta_3 = L, \\
L_1 \sin \theta_1 + L_2 \sin \theta_2 - L_3 \sin \theta_3 = 0, \\
\sin^2 \theta_1 + \cos^2 \theta_1 = 1, \\
\sin^2 \theta_2 + \cos^2 \theta_2 = 1, \\
\sin^2 \theta_3 + \cos^2 \theta_3 = 1.
\end{gathered}
$$
Observe that since we treat $sin\theta$ and $cos\theta$ as independent variables, we have included three trigonometric identities as independent equations.The basics physics says that because there are no acceleration, the sum of the forces in the horizontal and vertical directions must equal zero:
$$
\begin{gathered}
T_1 \sin \theta_1 - T_2 \sin \theta_2 - W_1 = 0, \\
T_1 \cos \theta_1 - T_2 \cos \theta_2 = 0, \\
T_2 \sin \theta_2 + T_3 \sin \theta_3 - W_2 = 0, \\
T_2 \cos \theta_2 - T_3 \cos \theta_3 = 0.
\end{gathered}
$$
Here $W_i$ is the weight of mass i and $T_i$ is the tension in string i. Note that because we do not have a rigid structure, we cannot assume an equilibrium of torques.

Equations are nine nonlinear equations, which being nonlinear cannot be solved with linear algebra. However, you can extend teh Newton-Raphson algorithm to multiple equations,and search for a solution.We rename the nine unknown angles and tension as the subscripted variable $y_i$, and place all of the variables into a vector:
$$
\boldsymbol{y}
=
\begin{bmatrix}
x_1 \\
x_2 \\
x_3 \\
x_4 \\
x_5 \\
x_6 \\
x_7 \\
x_8 \\
x_9
\end{bmatrix}
=
\begin{bmatrix}
\sin\theta_1 \\
\sin\theta_2 \\
\sin\theta_3 \\
\cos\theta_1 \\
\cos\theta_2 \\
\cos\theta_3 \\
T_1 \\
T_2 \\
T_3
\end{bmatrix}.
$$
The nine equation to be solvefe are written in a general form with zeros on the right-hand sides(RHS), and also placed in a vector:
$$
f_i(x_1,x_2,\ldots,x_N)=0,\quad i=1,N,
$$

$$
\mathbf{f}(\mathbf{y})
=
\begin{bmatrix}
f_1(\mathbf{y}) \\
f_2(\mathbf{y}) \\
f_3(\mathbf{y}) \\
f_4(\mathbf{y}) \\
f_5(\mathbf{y}) \\
f_6(\mathbf{y}) \\
f_7(\mathbf{y}) \\
f_8(\mathbf{y}) \\
f_9(\mathbf{y})
\end{bmatrix}
=
\begin{bmatrix}
3x_4+4x_5+4x_6-8 \\
3x_1+4x_2-4x_3 \\
x_7x_1-x_8x_2-10 \\
x_7x_4-x_8x_5 \\
x_8x_2+x_9x_3-20 \\
x_8x_5-x_9x_6 \\
x_1^2+x_4^2-1 \\
x_2^2+x_5^2-1 \\
x_3^2+x_6^2-1
\end{bmatrix}
=
\mathbf{0}.
$$

In words, we are looking for set of nine $x_i$ values for which all nine $f_i$ vanish.Although these equations are not very complicated, the terms quadratic in x make them nonlinear.

The search procedure guesses a solution, expands the nonlinear equations and keeps just the linear terms, solves the linear equations, and makes a better guess based on how close the previous guess was to making $f=0$. The search starts with the approximate solution at any one stage called the set $x_i$, and assumes that there ate corrections $\Delta x_i$ for which
$$
f_i(x_1+\Delta x_1, x_2+\Delta x_2, \ldots, x_9+\Delta x_9)=0,
\quad i=1,9.
$$
We solve for the approximate $\Delta x_i$ by assuming that our previous solution is close enough to the actual one for two terms in the Talor series to be accuate:
$$
f_i(x_1+\Delta x_1,\ldots,x_9+\Delta x_9)
\simeq
f_i(x_1,\ldots,x_9)
+
\sum_{j=1}^{9}
\frac{\partial f_i}{\partial x_j}
\Delta x_j
=0,
\quad i=1,9.
$$
We now have a sey of nine linear equations in the nine unknowns $\Delta x_i$, which we express as a single martrix equation
$$
\begin{aligned}
f_1
+ \frac{\partial f_1}{\partial x_1}\Delta x_1
+ \frac{\partial f_1}{\partial x_2}\Delta x_2
+ \cdots
+ \frac{\partial f_1}{\partial x_9}\Delta x_9
&=0,\\
f_2
+ \frac{\partial f_2}{\partial x_1}\Delta x_1
+ \frac{\partial f_2}{\partial x_2}\Delta x_2
+ \cdots
+ \frac{\partial f_2}{\partial x_9}\Delta x_9
&=0,\\
&\vdots\\
f_9
+ \frac{\partial f_9}{\partial x_1}\Delta x_1
+ \frac{\partial f_9}{\partial x_2}\Delta x_2
+ \cdots
+ \frac{\partial f_9}{\partial x_9}\Delta x_9
&=0.
\end{aligned}
$$

or

$$
\begin{bmatrix}
f_1\\
f_2\\
\vdots\\
f_9
\end{bmatrix}
+
\begin{bmatrix}
\dfrac{\partial f_1}{\partial x_1} & \dfrac{\partial f_1}{\partial x_2} & \cdots & \dfrac{\partial f_1}{\partial x_9}\\
\dfrac{\partial f_2}{\partial x_1} & \dfrac{\partial f_2}{\partial x_2} & \cdots & \dfrac{\partial f_2}{\partial x_9}\\
\vdots & \vdots & \ddots & \vdots\\
\dfrac{\partial f_9}{\partial x_1} & \dfrac{\partial f_9}{\partial x_2} & \cdots & \dfrac{\partial f_9}{\partial x_9}
\end{bmatrix}
\begin{bmatrix}
\Delta x_1\\
\Delta x_2\\
\vdots\\
\Delta x_9
\end{bmatrix}
=
0.
$$
Note now that the derivatives and the $f$'s are all evaluated at known values of the $x_i$, so taht only the vector of the $\Delta x_i$ values is unknown. We write this equation in matrix notation as
$$
\mathbf{f}+F'\Delta \mathbf{x}=0,
\qquad \Rightarrow \qquad
F'\Delta \mathbf{x}=-\mathbf{f},
$$

$$
\Delta \mathbf{x}
=
\begin{bmatrix}
\Delta x_1\\
\Delta x_2\\
\vdots\\
\Delta x_9
\end{bmatrix},
\qquad
\mathbf{f}
=
\begin{bmatrix}
f_1\\
f_2\\
\vdots\\
f_9
\end{bmatrix},
\qquad
F'
=
\begin{bmatrix}
\partial f_1/\partial x_1 & \cdots & \partial f_1/\partial x_9\\
\partial f_2/\partial x_1 & \cdots & \partial f_2/\partial x_9\\
\vdots & & \vdots\\
\partial f_9/\partial x_1 & \cdots & \partial f_9/\partial x_9
\end{bmatrix}.
$$

The equation $F'\Delta x=-f$ is in the standard form for the solution of a linear equation. In a formal sense, the solution of equation is obtained vy multiplying both sides of the equation by the inverse of the $F'$ matrix:
$$
\Delta \mathbf{x} = -F'^{-1}\mathbf{f},
$$


### 7.2 Matrix Generalities

Many programming bugs arise from the improper use of arrays. This may be as a result of the extensive use of matrices in scientific computing, or to the complexity of keeping track of indices and dimensions. In any case, here are some rules or thunmb to observe:

1. **Tests**: