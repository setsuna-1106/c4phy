#include <stdio.h>
#include <math.h>
#include "../common/rk4_2d.h"

#define h 1e-5
#define y0 20

double t;

/* TODO(P1): theta(b) 得到后计算微分截面 sigma(theta) = |dtheta/db| * b/sin(theta)
 *   (见笔记 13.3 节), 目前完全未实现 */
/* TODO(P3): 积分区间 y=±20 与固定步长 h=1e-5 可接受, 但可考虑靠近势阱区用自适应步长 */

typedef void (*deriv4)(double t, double y[4], double dydt[4]);

void rk4_4d(deriv4 f, double y[4], double t, double dt) {
    double k1[4], k2[4], k3[4], k4[4], yt[4];
    int i;

    f(t, y, k1);
    for (i = 0; i < 4; i++) yt[i] = y[i] + 0.5 * dt * k1[i];
    f(t + 0.5 * dt, yt, k2);
    for (i = 0; i < 4; i++) yt[i] = y[i] + 0.5 * dt * k2[i];
    f(t + 0.5 * dt, yt, k3);
    for (i = 0; i < 4; i++) yt[i] = y[i] + dt * k3[i];
    f(t + dt, yt, k4);

    for (i = 0; i < 4; i++)
        y[i] += dt / 6.0 * (k1[i] + 2.0 * k2[i] + 2.0 * k3[i] + k4[i]);
}

void deriv(double t,double *y,double *dydt){
    dydt[0]=y[1];
    dydt[1]=-2*y[2]*y[2]*y[0]*(1-y[0]*y[0])*exp(-y[0]*y[0]-y[2]*y[2]);
    dydt[2]=y[3];
    dydt[3]=-2*y[0]*y[0]*y[2]*(1-y[2]*y[2])*exp(-y[0]*y[0]-y[2]*y[2]);
}

void step(double *y,double dt){
    rk4_4d(deriv,y,t,dt);
    t+=dt;
}

void init(double *p,double x,double xv,double y,double yv){
    t=0;
    p[0]=x;
    p[1]=xv;
    p[2]=y;
    p[3]=yv;
}

int main(){
    double p[4];
    FILE *fp=fopen("b-theta.csv","w");
    for(int i=1;i<=1000;i++){
        double x0=(double)i/1000*2;
        init(p,x0,0,-y0,2);
        while(p[2]<=y0) step(p,h);
        double theta=atan2(p[1],p[3]);
        fprintf(fp,"%lf,%lf\n",x0,theta);
    }
    return 0;
}