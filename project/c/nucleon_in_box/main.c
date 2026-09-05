#include <stdio.h>
#include <math.h>

#include "../common/rk4_2d.h"

#define hbar 1
#define m 1
#define a 2
#define V0 83

#define h 5e-4 

double k;
double xm=1.1*a;
double x0=10;

double potential_func(double x){
    if(fabs(x)<=a) return -V0;
    else return 0;
}

void deriv(double t,double *y,double *dydt){
    dydt[0]=y[1];
    dydt[1] = (k*k + 2.0*m/(hbar*hbar)*potential_func(t)) * y[0];
}

double mis_func(double ka){
    k=ka;
    double x1=-x0,x2=x0;
    double y1[2],y2[2];
    y1[0]=exp(-k*x0);
    y1[1]=y1[0]*k;
    y2[0]=exp(-k*x0);
    y2[1]=-y2[0]*k;
    int nL=(int)((x0+xm)/h+0.5); 
    int nR=(int)((x0-xm)/h+0.5);   
    for(int i=0;i<nL;i++){ rk4_2d(deriv,y1,x1, h); x1+=h; }
    for(int i=0;i<nR;i++){ rk4_2d(deriv,y2,x2,-h); x2-=h; }
    return y1[1]*y2[0] - y1[0]*y2[1];
}

int main(void){
    double kmax=sqrt(2.0*m*V0/(hbar*hbar));  /* E=-V0 处, 约 12.884 */
    double dk=5e-3;
    double ka=1e-2,fa=mis_func(ka);
    int n=0;
    for(double kb=ka+dk; kb<kmax; kb+=dk){
        double fb=mis_func(kb);
        if(fa*fb<0){   
            double lo=ka,hi=kb,flo=fa;
            while(hi-lo>1e-9){                
                double mid=0.5*(lo+hi);
                double fm=mis_func(mid);
                if(flo*fm<=0) hi=mid;
                else { lo=mid; flo=fm; }
            }
            double kappa=0.5*(lo+hi);
            n++;
            printf("state %2d:  kappa = %9.6f,  E = %11.6f\n",
                   n,kappa,-0.5*kappa*kappa);
        }
        ka=kb; fa=fb;
    }
    printf("total bound states: %d\n",n);
    return 0;
}