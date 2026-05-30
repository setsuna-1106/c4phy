#ifndef BISECTION_H
#define BISECTION_H

double bisection(double (*fp)(double), double a, double b, double eps);

/* 每次调用 bisection 后，此变量保存实际迭代次数 */
extern int bisection_iters;

#endif
