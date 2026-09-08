#include <stdio.h>
#include <math.h>
#include "../common/rk4_4d.h"

#define h 1e-5
#define y0 20
#define V0 0.5  /* E = V0*V0/2 = 0.125, 需低于势垒顶 e^-2 ~= 0.135, 偏转角才会出现不连续 */
#define TMAX 1000 /* 近俘获轨道的最大积分时间, 防止死循环 */

double t;

/* TODO(P1): theta(b) 得到后计算微分截面 sigma(theta) = |dtheta/db| * b/sin(theta)
 *   (见笔记 13.3 节), 目前完全未实现 */
/* TODO(P3): 积分区间 y=±20 与固定步长 h=1e-5 可接受, 但可考虑靠近势阱区用自适应步长 */

void deriv(double t,double *y,double *dydt){
    (void)t; /*自治系统, 不显含时间*/
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
    if(fp==NULL){
        perror("fopen b-theta.csv");
        return 1;
    }
    for(int i=1;i<=1000;i++){
        double x0=(double)i/1000*2;
        init(p,x0,0,-y0,V0);
        /* 偏转角逐级解卷绕累积; 直接 atan2 终值会折叠进 (-pi,pi], 抹掉整圈缠绕 */
        double theta=0,phi_prev=atan2(p[1],p[3]);
        while(fabs(p[2])<=y0&&t<TMAX){
            step(p,h);
            double phi=atan2(p[1],p[3]);
            double d=phi-phi_prev;
            if(d>M_PI)d-=2*M_PI;
            if(d<-M_PI)d+=2*M_PI;
            theta+=d;
            phi_prev=phi;
        }
        fprintf(fp,"%lf,%lf\n",x0,theta);
    }
    return 0;
}