## Fourier Analyses

> This chapter dicusses Flourier series and Fourier transforms. Whenm implemented as algorithm, both become the Discrete Fourier Transform  (DFT), or its fast cousin, the Fast Fourier Transform (FFT).

### 9.1 Flourier Series

Your **problem** is to take the solution of one of these nonlinear oscillators and expand it in a Fourier series:
$$
y(t)=b_0sin\omega_0t+b_1sin2\omega_0t+...
$$
In general, when we undertake such a spectral analysis we want to analyze the steady-state behaviour of a system. This means that we have to wait for the initial transients to die out.

In classial mechanics that the general solution for a vibrating system can be expressed as the sum ofthe normal modes of that system. These expansions are possible only if we have *linear operators* and the *principle of superposition*. The principle of linear suerposition does not hold when we solve nonlinear problems. Nevertheless, it is always possible to expand a *periodic* solution of a nonlinear problem in terms of trigonometric functions. This is a consequence of *Fourier,s theorem* being applicable to any single-valued periodic function with only a finite number of discontinuties.

> 线性系统：简正模展开 → 求解（因为叠加原理）
>                             ↓
> 非线性系统：简正模展开 ✗（无叠加原理）
>             数值积分 → 解出 y(t)
>                             ↓
>             傅里叶展开 → 分析 y(t) 的频谱（纯数学工具，不依赖线性）

We assume we know the period $T$, that is
$$
y(t+T)=y(t)
$$
This tell us the ture frequency $\omega$:
$$
w_1=\frac{2\pi}{T}
$$
