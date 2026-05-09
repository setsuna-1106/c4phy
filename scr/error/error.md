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