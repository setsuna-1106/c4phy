#include <stdio.h>
#include <math.h>
#include "../common/rk4_2d.h"

#define h 1e-5

double x_[2],y_[2];
double t;

void derivx(double t,double *x,double *dxdt){
    dxdt[0]=x_[1];
    dxdt[1]=-2*y_[0]*y_[0]*x_[0]*(1-x_[0]*x_[0])*exp(-x_[0]*x_[0]-y_[0]*y_[0]);
}
void derivy(double t,double *y,double *dydt){
    dydt[0]=y_[1];
    dydt[1]=-2*x_[0]*x_[0]*y_[0]*(1-y_[0]*y_[0])*exp(-x_[0]*x_[0]-y_[0]*y_[0]);
}

void step(double dt){
    rk4_2d(derivx,x_,t,dt);
    rk4_2d(derivy,y_,t,dt);
    t+=dt;
}

void init(){
    x_[0]=3;
    x_[1]=0;
    y_[0]=-20;
    y_[1]=5;
    t=0;
}

int main(){
    init();
    while(y_[0]<=20){
        step(h);
    }
    printf("the tan is %lf",x_[1]/y_[1]);
}