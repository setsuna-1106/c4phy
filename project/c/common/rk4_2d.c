#include "rk4_2d.h"

/*经典四阶Runge-Kutta法, 对二维一阶ODE组从t积分到t+dt*/
void rk4_2d(deriv2 f, double y[2], double t, double dt) {
    double k1[2], k2[2], k3[2], k4[2], yt[2];
    int i;

    f(t, y, k1);
    for (i = 0; i < 2; i++) yt[i] = y[i] + 0.5 * dt * k1[i];
    f(t + 0.5 * dt, yt, k2);
    for (i = 0; i < 2; i++) yt[i] = y[i] + 0.5 * dt * k2[i];
    f(t + 0.5 * dt, yt, k3);
    for (i = 0; i < 2; i++) yt[i] = y[i] + dt * k3[i];
    f(t + dt, yt, k4);

    for (i = 0; i < 2; i++)
        y[i] += dt / 6.0 * (k1[i] + 2.0 * k2[i] + 2.0 * k3[i] + k4[i]);
}
