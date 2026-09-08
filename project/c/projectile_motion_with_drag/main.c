/*
 * TODO(按优先级评级):
 * [P0] 落地判定只做到 y<0 即 break: 最后一步的穿地数据被直接丢弃,
 *      应对最后一步线性插值求精确落点, 并输出射程与飞行时间.
 *
 * [P1] 阻力项写作 -k*m*|v|^n: 加速度中乘 m 属量纲混乱(m=1 时数值无差),
 *      应去掉 m 或明确 drag 系数与 F_drag = -k*v^n 的约定, 见 deriv().
 *
 * [P3] n=0.5 时 v^0.5 在 v→0 处导数发散, 顶点过零附近 RK4 局部精度下降;
 *      若需高精度应局部缩小步长(或改用 n=1/2 的常规阻力指数).
 */

#include <stdio.h>
#include <math.h>
#include "../common/rk4_4d.h"

#define k 1
#define m 1
#define g 9.8
#define n 0.5

#define h 1e-3
#define counter 10000

double t;

/* TODO[P1]: 加速度中的阻力不应乘 m, 详见文件头 */
void deriv(double t,double *y,double *dydt){
    (void)t; /*自治系统, 不显含时间*/
    double v=sqrt(y[1]*y[1]+y[3]*y[3]);
    dydt[0]=y[1];
    dydt[1]=-k*pow(v,n)*y[1]/v/m;
    dydt[2]=y[3];
    dydt[3]=-k*pow(v,n)*y[3]/v/m-g;
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
    if(fp==NULL){
        perror("fopen x-y.csv");
        return 1;
    }
    double p[4];
    init(p,0,5,0,10);
    for(int i=0;i<counter;i++){
        step(p,h);
        if(p[2]<0) break;
        fprintf(fp,"%lf,%lf\n",p[0],p[2]);
    }
    return 0;
}
