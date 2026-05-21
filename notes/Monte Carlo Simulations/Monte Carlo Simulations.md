## Monte Carlo Simulation

### Random Number

We define a sequence *r1,r2,...,as random* if there are no correlations among the numbers.

#### Random Number Generation

The linear congruent or power residue method is the common way of generating a pseudorandom sequence of numbers 0 ≤ ri ≤ M − 1 over the interval [0, M − 1].

this is a example of linear congruent:

![Monte Carlo Simulation1](/Users/setsuna/Desktop/c4phy/notes/Monte Carlo Simulations/Monte Carlo Simulations.assets/Monte Carlo Simulation1.png)As a rule of thumb: Before using a random-number generator in your programs, you should check its range and that it produces numbers that “look” random.

#### 4.1.2 Computing a Random Sequence

For scientific work we recommend using an industrial-strength random-number generator.

### 4.2 Simulating a Random Walk

We will present a simple model for a 2D walk, and end up with a model for normal diffusion. 

In our random-walk simulation an artificial walker takes sequential steps with the direction of each step independent of the direction of the previous step. 

![截屏2026-05-14 00.02.43](/Users/setsuna/Desktop/c4phy/notes/Monte Carlo Simulations/Monte Carlo Simulations.assets/Monte Carlo Simulation2.png)

#### Random Protein Folding

The walk is restricted such that the only positions available after each step are the three neighboring sites, with the already occupied sites excluded (this is why this technique is known as a self-avoiding random walk).

The goal of the simulation is to find the lowest energy state of a sequence of H and P monomers with links of various lengths.

The energy of a chain is defined as $E=-\epsilon f$

where 𝜖 is a positive constant and f is the number of H–H neighbor not connected directly(P–P and H–P bonds do not count at lowering the energy).

### 4.3 Spontaneous Decay

Your problem is to simulate the time dependence of the decay of a small number N of radioactive particles.

Spontaneous decay is a natural process in which a particle, with no external stimulation, decays into other particles.

#### 4.3.1 Discrete Decay Model

We convert the statement “the probability $P$ of any one particle decaying per unit time is a constant” into the equation
$$
P=\frac{\Delta N(t)/N(t)}{\Delta t}=-\lambda
$$
where the constant 𝜆 is called the decay rate and the minus sign indicates a decreasing number.[Actually, because the number of decays ΔN(t) is proportional to the difference in random numbers, it tends to show even larger statistical fluctuations than does N(t).]

#### 4.3.2 The Exponential Decay Approximation

When the number of particles N → ∞ and the observation time interval Δt → 0, the difference equation (4.21) becomes a differential equation,nd we obtain the familiar exponential decay law:
$$
\frac{\Delta N(t)}{\Delta t}\rightarrow \frac{dN(t)}{dt}=-\lambda N(t)
$$
This equation can be integrated to obtain the time dependencies of the total number of particles and of the total activity:
$$
N(t)=N(0)e^{-\lambda t}=N(0)e^{-t/\tau}
$$


#### 4.3.3 Discrete Decay Simulation

the example is in Spontaneous decay



### 4.4 Testing and Generating Random Distributions

Since the computer’s random numbers are generated according to a definite rule, they must be correlated with each other. *This can affect a simulation that assumes truly random events.*
Therefore it is wise to test a random-number generator to obtain a numerical measure of its uniformity and randomness before you stake your scientific reputation on it.

##### A simple test of uniformity

A simple test of uniformity evaluates the kth moment of a distribution:
$$
\langle x^k \rangle = \frac{1}{N} \sum_{i=1}^{N} x_i^k
$$
If the numbers are distributed uniformly, then (4.26) is approximately the moment of the distribution function P(x):
$$
\frac{1}{N}\sum_{i=1}^{N} x_i^k
\simeq
\int_{0}^{1} dx\, x^k P(x)
\simeq
\frac{1}{k+1}
+
O\!\left(\frac{1}{\sqrt{N}}\right).
$$
*If holds for your generator, then you know that the distribution is uniform.* 

You can see the test in ex1.c

##### A simple test of the near-neighbour correlation in random sequence

By taking sums of products for small k:
$$
C(k)=\frac{1}{N}\sum^N_{i=1}x_ix_{i+k},\quad(k=1,2,...).
$$
If your random numbers $x_i$ and $x_{i+k}$ are distributed with the joint probability distribution $P(x_i, x_{i+k}) = 1$ and are independent and uniform, then it can be approximated as an integral:
$$
\frac{1}{N}\sum_{i=1}^{N} x_i x_{i+k}
\simeq
\int_{0}^{1} dx \int_{0}^{1} dy\, xy\, P(x,y)
=
\int_{0}^{1} dx \int_{0}^{1} dy\, xy
=
\frac{1}{4}.
$$
You can see the test in ex2.c

