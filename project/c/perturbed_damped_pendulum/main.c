#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "rk4.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define Gamma 1  /*阻尼系数*/
#define omega0 1 /*固有角频率*/
#define Omega 1  /*驱动力角频率*/
#define F 1      /*驱动力幅值*/
#define dt 1e-2  /*时间步长*/
#define N 10000  /*总步数, 模拟到t=100*/

double theta;
double omega;
double t;

/*受迫阻尼摆的右端函数: y = (θ, ω)*/
void deriv(double t, double y[2], double dydt[2]) {
    dydt[0] = y[1];
    dydt[1] = -omega0 * omega0 * sin(y[0]) - Gamma * y[1] + F * sin(Omega * t);
}

/*随机生成初始角位移与角速度*/
void init() {
    srand(time(NULL));
    theta = 2.0 * M_PI * rand() / RAND_MAX - M_PI; /*θ ∈ [-π, π)*/
    omega = 10.0 * rand() / RAND_MAX - 5.0;         /*ω ∈ [-5, 5)*/
    t = 0;
}

/*用RK4推进一步, 并把θ卷绕到(-π, π]以便绘制相图*/
void step(double t1) {
    double y[2] = {theta, omega};

    rk4(deriv, y, t, t1);
    omega = y[1];
    theta = fmod(y[0] + M_PI, 2.0 * M_PI);
    if (theta < 0) theta += 2.0 * M_PI;
    theta -= M_PI;
    t += t1;
}

int main() {
    init();
    FILE *fp = fopen("theta-omega.csv", "w");
    FILE *fp2 = fopen("theta-t.csv", "w");
    fprintf(fp, "theta,omega\n");
    fprintf(fp2, "t,theta\n");
    for (int i = 0; i < N; i++) {
        step(dt);
        fprintf(fp, "%lf,%lf\n", theta, omega);
        fprintf(fp2, "%lf,%lf\n", t, theta);
    }
    fclose(fp);
    fclose(fp2);
    printf("finish the simulation, the data is in theta-t.csv and theta-omega.csv\n");
    return 0;
}
