#include <stdio.h>
#include <math.h>

#define SIGN(x) ((x) > 0 ? 1 : ((x) < 0 ? -1 : 0))

#define k 1
#define m 1
#define g 9.8
#define n 0.5

#define h 1e-2

double t;

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
    dydt[1]=-k*m*pow(fabs(y[1]),n)*SIGN(y[1]);
    dydt[2]=y[3];
    dydt[3]=-k*m*pow(fabs(y[3]),n)*SIGN(y[3])-m*g;
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
    FILE *fp=fopen("x-y.csv","w");
    double p[4];
    init(p,0,5,0,10);
    for(int i=0;i<1e5;i++){
        step(p,h);
        fprintf(fp,"%lf,%lf\n",p[0],p[2]);
    }
    return 0;
}