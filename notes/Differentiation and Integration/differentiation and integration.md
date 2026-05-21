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