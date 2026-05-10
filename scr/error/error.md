---
typora-copy-images-to: ./error
---

## Types of Errors

Computer errors arise because of the limited precision with which computers store numbers, or because algorithms or models are not perfect. 

There are four general types of errors exist to plague your computations:

1. *Blunders or bad theory*: typographical errors entered with your program or data, running the wrong program, or having a fault in your reasoning (theory), using the wrong data file, and so on.
2. *Random errors*: imprecision caused by events such as fluctuations in electronics, cosmic rays, or someone pulling a plug. 
3. *Approximation erroes*:imprecision arising from simplifying the mathematics so that a problem can be solved on the computer. 
4. *Round-off errors*: imprecision arising from the finite number of digits used to store floating-point numbers. 



### Courting Disater : Subtractive Cancelation

*Theorem* If you subtract two large numbers and end up with a small one, the small one is less significant than the large numbers.

you can see some emaxples of subtrctive cancelation in ex1.c,ex2.c,ex3.c

### Round-Off Errors

we can use the difference in the value of the function evaluated at x and at x_c

#### Round-Off Error Accumlation

There is a useful model for approximating how round-off error accumulates in a calculation involving a large number of steps. -- **Random Walk**

By analogy, the total relative error 𝜖_a arising after N calculational steps each with machine precision error 𝜖_m is, on the average

If the round-off errors in a particular algorithm do not accumulate in a random manner, then a detailed analysis is needed to predict the dependence of the error on the number of steps N. In some cases, there may be no cancellation, and the error may increase as N𝜖m. Even worse, in some recursive algorithms, where the error generation is coherent, such as the upward recursion for spherical Bessel functions, there may be an N! increase in error.



### Experimental Error Investigation

Algorithms play a vital role in computational physics.

The problem is to take an alogorithm and decide

1. Does it converge,and if so,how fast?
2. Even if it converge,are the answers precise?
3. How expensive (time-consuming) is the algorithm?

Good algorithms are good not only because fewer steps take less time, but also because fewer steps produces less round-off error.

The total error in a computation is the sum of the two types of errors : approximation (algorithmic) error and round-off error
For small N we expect the first term to be the larger of the two, but as N grows it will be overcome by the ever-increasing round-off error.

When the number of steps becomes even larger,round-off error leads to a fluctuation in the less significant digits, with a gradual increase on the average. It is best to quit the calculation before this occurs.

In this case, the better algorithm is quicker and, by using fewer steps, produces less round-off error.

### Errors with Power Series

A classic numerical problem is the summation of a series to evaluate a function. As an example, consider the infinite series for sin x:

One approach to decide when to stop summing would be to stop summing when the next term is smaller than the percision desired.

![截屏2026-05-10 15.33.42](/Users/setsuna/Desktop/c4phy/scr/error/error/error1.png)

But there are two main problem of this classic algorithm.
On the one hand, both (2n − 1)! and $x^{2n−1}$ can individually get very large and thereby cause overflows, despite the fact that their quotient may be small. On the other hand, powers and factorials are very expensive (time-consuming) to evaluate on the computer. 

Consequently, a better approach is to use a single multiplication to relate the next term in the series to the previous one.

you can see the algorithm in ex4.c

