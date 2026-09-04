#ifndef RK4_2D_H
#define RK4_2D_H

/*二维一阶常微分方程组的右端函数类型: dydt = f(t, y)*/
typedef void (*deriv2)(double t, double y[2], double dydt[2]);

/*用四阶Runge-Kutta法把y推进一个时间步dt, 就地更新y[2]*/
void rk4_2d(deriv2 f, double y[2], double t, double dt);

#endif
