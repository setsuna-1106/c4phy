#include <stdio.h>
#include <math.h>

#include "../common/rk4_2d.h"

#define hbar 1
#define m 1
#define a 2
#define V0 83

#define h 1e-3
#define tolerance 1e-5

double k;// eigenvalue
double xm=1.1*a;
double x0=10;

double potential_func(double x){
    if(fabs(x)<=a) return -V0;
    else return 0;
}

void deriv(double t,double *y,double *dydt){
    dydt[0]=y[1];
    dydt[1]=k*k-2*m/h/h*potential_func(t);
}

double mis_func(){
    double x1=-x0,x2=x0;
    double y1[2],y2[2];
    y1[0]=exp(x1);
    y1[1]=y1[0]*k;
    y2[0]=exp(x2);
    y2[1]=y2[0]*k;
    while(x1<xm){
        rk4_2d(deriv,y1,x1,h);
        x1+=h;
    }
    while(x2>xm){
        rk4_2d(deriv,y2,x2,h);
        x2-=h;
    }
    double ld_l,ld_r;
    ld_l=y1[1]/y1[0];
    ld_r=y2[1]/y2[0];
    return (ld_l-ld_r)/(ld_l+ld_r);
}

int main(){
    k=1e-4;
    while(mis_func()>tolerance){
        k+=1e-5;
    }
    printf("%.2lf",k);
    return 0;
}