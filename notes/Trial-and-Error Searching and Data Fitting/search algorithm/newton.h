#ifndef NEWTON_H
#define NEWTON_H

double newton_search(double (*fp)(double), double a, double eps);

/* 每次调用 newton_search 后，此变量保存实际迭代次数。
   若达到上限（100）仍未收敛，返回 NAN 并设 newton_failed = 1。 */
extern int newton_iters;
extern int newton_failed;

#endif
