#ifndef RK4_1D_H
#define RK4_1D_H

typedef double (*ODE_Function)(double t, double y);

double rk4_1d(ODE_Function f, double t, double y, double h);

#endif