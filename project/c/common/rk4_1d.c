#include "rk4_1d.h"

double rk4_1d(ODE_Function f, double t, double y, double h){
    double k1 = h * f(t, y);
    double k2 = h * f(t + h / 2.0, y + k1 / 2.0);
    double k3 = h * f(t + h / 2.0, y + k2 / 2.0);
    double k4 = h * f(t + h, y + k3);
    
    return y + (k1 + 2.0 * k2 + 2.0 * k3 + k4) / 6.0;
}
