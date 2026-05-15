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