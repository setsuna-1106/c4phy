## 5 Differentiation and Integration

------

> [!IMPORTANT]
>
> We start this chapter with a short discussion of numerical differentiation, an important, if rather straight-forward, topic. We derive the algorithms for differentiation that will be used throughout the book. The majority of the chapter covers several algorithms for numerical integration, a basic tool of scientific computation. We end with a discussion of Monte Carlo integration techniques, which are fundamentally different from all the others.

### 5.1 Differentiation Algorithms

In calculus course we usually take derivatives to get tangent line.In fact, even a computer will have problems with this kind of limit because it is wrought with subtractive cancellation when we take derivatives.

#### 5.1.1 Forward Difference

The most direct method for numerical differentiation starts by expanding a function in a Taylor series to obtain its value at a small step h away:
$$
y(t+h)=y(t)+h\frac{dy(t)}{dt}+\frac{h^2}{2!}\frac{d^2y(t)}{dt^2}+...
$$
If we ignore the h2 terms in (1) , we obtain the forward-difference algorithm for the derivative:
$$
\left.
\frac{dy(t)}{dt}
\right|_{\mathrm{fd}}
\overset{\mathrm{def}}{=}
\frac{y(t+h)-y(t)}{h}.
$$
An estimate of the error follows from substituting the Taylor series (1):
$$
\left.
\frac{dy(t)}{dt}
\right|_{\mathrm{fd}}
\simeq
\frac{dy(t)}{dt}
+
\frac{h}{2}\frac{d^2y(t)}{dt^2}
+\cdots.
$$

#### 5.1.2 Central Difference

 Now, rather than making a single step of h forward, we form a central difference by stepping forward half a step and backward half a step:
$$
\left.
\frac{dy(t)}{dt}
\right|_{\mathrm{cd}}
\equiv
D_{\mathrm{cd}}\,y(t)
\overset{\mathrm{def}}{=}
\frac{y(t+h/2)-y(t-h/2)}{h}.
$$
![截屏2026-05-20 12.20.40](/Users/setsuna/Desktop/c4phy/notes/Differentiation and Integration/differentiation and integration.assets/dai1.png)

We estimate the error in the central-difference algorithm by substituting the Taylor series for y(t + h∕2) and y(t − h∕2) into
$$
\begin{aligned}
y\left(t+\frac{h}{2}\right)-y\left(t-\frac{h}{2}\right)
&\simeq
\left[
y(t)
+
\frac{h}{2}y'(t)
+
\frac{h^2}{8}y''(t)
+
\frac{h^3}{48}y'''(t)
+
\mathcal{O}(h^4)
\right]
\\
&\quad -
\left[
y(t)
-
\frac{h}{2}y'(t)
+
\frac{h^2}{8}y''(t)
-
\frac{h^3}{48}y'''(t)
+
\mathcal{O}(h^4)
\right]
\\
&=
h y'(t)
+
\frac{h^3}{24}y'''(t)
+
\mathcal{O}(h^5),
\\[6pt]
\Rightarrow\quad
\left.
\frac{dy(t)}{dt}
\right|_{\mathrm{cd}}
&\simeq
y'(t)
+
\frac{1}{24}h^2 y'''(t)
+
\mathcal{O}(h^4).
\end{aligned}
$$


### 5.2 Extrapolated Difference

we can reduce the theoretical error further by forming a combination of approximations whose summed errors extrapolate to zero. A second algorithm is another central-difference approximation, but this time using quarter-steps

A combination of the two ,called the *extended difference algorithm*,eliminates both the quadratic and linear terms:
$$
\left.
\frac{dy(t)}{dt}
\right|_{\mathrm{ed}}
\overset{\mathrm{def}}{=}
\frac{
4D_{\mathrm{cd}}y(t,h/2)-D_{\mathrm{cd}}y(t,h)
}{3}
$$

$$
\simeq
\frac{dy(t)}{dt}
-
\frac{h^4 y^{(5)}(t)}{4\times 16\times 120}
+\cdots .
$$

Equation ( 6 ) is the *extend-difference algorithm* and ( 7 ) is its error.

When working with these, and similar higher-order methods, it is important to remember that while they may work as designed for well-behaved functions, they may fail badly for functions containing noise, as do data from computations or measurements. If noise is evident, it may be better to first smooth the data.

#### 5.2.1 Second Derivates

$$
\begin{aligned}
\left.
\frac{d^2 y(t)}{dt^2}
\right|_{\mathrm{cd}}
&\simeq
\frac{
y'\left(t+h/2\right)-y'\left(t-h/2\right)
}{h}
\\[6pt]
&\simeq
\frac{
\left[y(t+h)-y(t)\right]
-
\left[y(t)-y(t-h)\right]
}{h^2}
\\[6pt]
\end{aligned}
$$

$$
=
\frac{
y(t+h)+y(t-h)-2y(t)
}{h^2}.
$$

Although the form ( 9 ) is more compact and requires fewer steps than ( 8 ), it may increase subtrative cancellation by first storing the "large" number,and then subtracting another large number feom it.



### 5.3 Integration Algorithm

#### 5.3.1 Box Counting

The Riemann definition of an integral is the limit of the sum over boxes as the width h of the box approaches zero.
$$
\int_a^b f(x)\,dx
=
\lim_{h\to 0}
\left[
h\sum_{i=1}^{(b-a)/h} f(x_i)
\right].
$$
The numerical integral of a function $f(x)$ is approximated as the equivalent of a finite sum over boxes of height $f(x)$ and width $w_i$:
$$
\int_a^b f(x)\,dx
\simeq
\sum_{i=1}^{N} f(x_i)w_i.
$$
Equation( 11 ) is the standard from for all integration algorithms

In general, you should not attempt a numerical integration of an integrand that contains a singularity without first somehow removing the singularity. You may be able to do this very simply by *breaking the interval down into several subintervals* so the singularity is at an endpoint where an integration point is not placed, or by a *change of variable* 

Likewise, if your integrand has a very slow variation in some region, you can speed up the integration by changing to a variable that compresses that region and places few points there, or divides up the interval and performs several integrations. Conversely, if your integrand has a very rapid variation in some region, you may want to change to variables that expand that region to ensure that no oscillations are missed.



#### 5.3.2 Trapezoid Rule

The trapezoid and Simpson's integration rules both use evenly spaced values of x.They use N points $x_i ,i=1,N$,evenly spaced a distance h apart throught the integration region $[a,b]$,and include teh endpoints in the integration region.This means that there are (N-1) intervals, each of length h:
$$
h=\frac{b-a}{N-1},
\qquad
x_i=a+(i-1)h,
\qquad
i=1,\ldots,N.
$$
The trapezoid rule takes each integration interval i and constructs a trapezoid of width h in it.This approximates $f(x)$ by a straight line in each interval i,and uses the average height$(f_i+f_{i+1})/2$ as the value for $f$

![截屏2026-05-22 16.44.58](/Users/setsuna/Desktop/c4phy/notes/Differentiation and Integration/differentiation and integration.assets/截屏2026-05-22 16.44.58.png)

The area of each such trapezoid is:
$$
\int_{x_i}^{x_i+h} f(x)\,dx
\simeq
\frac{h(f_i+f_{i+1})}{2}
=
\frac{1}{2}h f_i
+
\frac{1}{2}h f_{i+1}.
$$
In order to apply the trapezoid rule to the entrie rigion $[a,b]$, we add the contributions from ezch subinterval:
$$
\int_a^b f(x)\,dx
\simeq
\frac{h}{2}f_1
+
h f_2
+
h f_3
+
\cdots
+
h f_{N-1}
+
\frac{h}{2}f_N.
$$
we have
$$
w_i=\{\frac h2,h,...,h,\frac h2\}\qquad(Trapezoid\ rule).
$$

#### 5.3.3 Simpson's Rule

Simpson's rule approximates the integrand $f(x)$ by a parabola within each equally spaced interal
$$
f(x)\simeq \alpha x^2+\beta x+\gamma.
$$
we evaluate the function at the two endpoints and in the middle
$$
\int_{x_i-h}^{x_i+h} f(x)\,dx
=
\int_{x_i}^{x_i+h} f(x)\,dx
+
\int_{x_i-h}^{x_i} f(x)\,dx
\simeq
\frac{h}{3}f_{i-1}
+
\frac{4h}{3}f_i
+
\frac{h}{3}f_{i+1}.
$$
Take notes: Simpson's rule requires the elementary integration to be over pairs of intervals

In order to apply Simpson’s rule to the entire interval, we add up the contributions from each pair of subintervals, counting all but the first and last endpoints twice:
$$
\int_a^b f(x)\,dx
\simeq
\frac{h}{3}f_1
+
\frac{4h}{3}f_2
+
\frac{2h}{3}f_3
+
\frac{4h}{3}f_4
+
\cdots
+
\frac{4h}{3}f_{N-1}
+
\frac{h}{3}f_N.
$$
In terms of our standard integration rule, we have
$$
w_i=\{\frac h3,\frac{4h}3,\frac{2h}3,\frac{4h}3,...,\frac{4h}3,\frac h3\}\qquad(Simpson's\ rule).
$$
Remember, the number of points N must be odd for Simpson's rule.

#### 5.3.4 Simple Integration Error Estimates

We can multiply that error by the number of intervals N to estimate the error for the entire region $[a,b]$. For the trapezoid and Simpson's rules this yields
$$
\mathcal{E}_t
=
O\!\left(
\frac{(b-a)^3}{N^2}
\right) f^{(2)},
\qquad
\mathcal{E}_s
=
O\!\left(
\frac{(b-a)^5}{N^4}
\right) f^{(4)},
\qquad
\mathcal{E}_{t,s}
=
\frac{\mathcal{E}_{t,s}}{f}.
$$
where $\epsilon$ is a measure of the relative error.

We want to determine an N that minimizes the total error,that is ,the sum of the approximation and round-off errors:
$$
\epsilon_{tot}=\simeq\epsilon_{ro}+\epsilon_{app}
$$
This occurs,approximately, when the two errors are equal magnitude, which we approximate even further by assuming that the two errors are equal:
$$
\epsilon_{\mathrm{ro}}
=
e_{\mathrm{app}}
=
\frac{\mathcal{E}_{\mathrm{trap,simp}}}{f}.
$$
To continue the search for optimum N for ageneral function f,we set the scale of function size and lengths by assuming
$$
\frac{f^{(n)}}{f} \simeq 1,
\qquad
b-a=1
\quad\Rightarrow\quad
h=\frac{1}{N}.
$$
The estimate, when applied to the trapezoid rule,yields
$$
\sqrt{N}\epsilon_m
\simeq
\frac{f^{(2)}(b-a)^3}{fN^2}
=
\frac{1}{N^2},
$$

$$
\Rightarrow\quad
N
\simeq
\frac{1}{(\epsilon_m)^{2/5}}
=
\left(
\frac{1}{10^{-15}}
\right)^{2/5}
=
10^6,
$$

$$
\Rightarrow\quad
\epsilon_{\mathrm{ro}}
\simeq
\sqrt{N}\epsilon_m
=
10^{-12}.
$$

The estimate,when applied to Simpson's rule, yields
$$
\sqrt{N}\epsilon_m
\simeq
\frac{f^{(4)}(b-a)^5}{fN^4}
=
\frac{1}{N^4},
$$

$$
\Rightarrow\quad
N
=
\frac{1}{(\epsilon_m)^{2/9}}
=
\left(
\frac{1}{10^{-15}}
\right)^{2/9}
=
2154,
$$

$$
\Rightarrow\quad
\epsilon_{\mathrm{ro}}
\simeq
\sqrt{N}\epsilon_m
=
5\times 10^{-14}.
$$

These result are illuminating in that they show how:

1. Simpson's rule requires fewer points and has less error than the trapezoid rule.
2. It is possible to obtian an error close to machine precision with Simpson's rule(and with other higher-order integration algorithms).
3. Obtaining the best numerical approximation to an integral is not achieved by letting N → ∞, but with a relatively small N ≤ 1000. Larger N only gives you more round-off errors.

you can see the estimate in main.c and visual.py in file integration/.

#### 5.3.5 Higher-Order Algorithms

As in numerical differentiation, we can use the known functional dependence of the error on interval size h to reduce the integration error.

To illustrate, if A(h) and A(h∕2) are the values of the integral determined for intervals h and h∕2, respectively, and if we assume that the numerical evaluation of the integral has an error whose expansion has a leading error term proportional to $h^2$
$$
A(h) \simeq \int_a^b f(x)\,dx + \alpha h^2 + \beta h^4 + \cdots,
$$

$$
\text{then}\qquad
A\left(\frac{h}{2}\right)
\simeq
\int_a^b f(x)\,dx
+
\frac{\alpha h^2}{4}
+
\frac{\beta h^4}{16}
+
\cdots .
$$

Consequently, we can make the $h^2$ term in the error vanish by computing the integral as the combination
$$
A
\simeq
\frac{4}{3}A\left(\frac{h}{2}\right)
-
\frac{1}{3}A(h)
\simeq
\int_a^b f(x)\,dx
-
\frac{\beta h^4}{4}
+
\cdots .
$$
![截屏2026-05-24 16.33.06](/Users/setsuna/Desktop/c4phy/notes/Differentiation and Integration/differentiation and integration.assets/截屏2026-05-24 16.33.06.png)

this is Romberg’s **extrapolation**.



### 5.4 Gaussian Quadrture

It is often useful to rewrite the basic integration formula with a weightinh function $W(x)$ separate from the integrand:
$$
\int_a^b f(x)\,dx
\equiv
\int_a^b W(x)g(x)\,dx
\simeq
\sum_{i=1}^{N} w_i g(x_i).
$$
In the Gaussian quadrature approach to integration, the N points and weights in (34) are chosen to make the integration exact if $g(x)$ were a $(2N-1)$degree polynomial.

To obtain this incredible optimization, the points $x_i$ end up having a specific distribution over $[a,b]$.

All the rules indicated in Table are form of Gaussian quadrature following the general form (34)![截屏2026-05-26 18.43.26](/Users/setsuna/Desktop/c4phy/notes/Differentiation and Integration/differentiation and integration.assets/截屏2026-05-26 18.43.26.png)



#### 5.4.1 Mapping Gaussian Points

Our standard integration rule for the general interval $[a,b]$ is
$$
\int^b_af(x)dx\simeq \sum^N_{i=1}f(x_i)w_i.
$$
With Gaussian points and weights, the y interval $-1<y_i\le1$ must be mapped onto the x interval $a\le x\le b$.
