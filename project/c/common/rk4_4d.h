#ifndef RK4_4D_H
#define RK4_4D_H

/*四维一阶常微分方程组的右端函数类型: dydt = f(t, y)*/
typedef void (*deriv4)(double t, double y[4], double dydt[4]);

/*用四阶Runge-Kutta法把y推进一个时间步dt, 就地更新y[4]*/
void rk4_4d(deriv4 f, double y[4], double t, double dt);

#endif
