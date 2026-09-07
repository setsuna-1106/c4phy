/*
 * TODO(按优先级评级):
 * [P0] 缺少落地判定: 固定积分 1e5 步至 t=1000s, 弹体 t≈1.83s 已落地,
 *      输出 CSV 中 99.8% 为穿地坠落的无效数据; 应检测 y<0 终止, 并对
 *      最后一步线性插值求精确落点, 输出射程与飞行时间.
 *
 * [P1] 阻力项写作 -k*m*|v|^n: 加速度中乘 m 属量纲混乱(m=1 时数值无差),
 *      应去掉 m 或明确 drag 系数与 F_drag = -k*v^n 的约定, 见 deriv().
 *
 * [P1] 缺 Makefile: 仓库其他项目(如 classical_chaotic_scattering)规范为
 *      -Wall -Wextra -O2 -MMD -MP + build/ 目录 + run/clean 目标;
 *      复用 common/rk4_4d 需 -I../common 并链接 rk4_4d.c.
 *
 * [P2] deriv() 的参数 t 未使用, 触发 -Wunused-parameter 警告;
 *      自治系统可 (void)t 消除.
 *
 * [P3] n=0.5 时 v^0.5 在 v→0 处导数发散, 顶点过零附近 RK4 局部精度下降;
 *      若需高精度应局部缩小步长(或改用 n=1/2 的常规阻力指数).
 */

#include <stdio.h>
#include <math.h>
#include "rk4_4d.h"

#define SIGN(x) ((x) > 0 ? 1 : ((x) < 0 ? -1 : 0))

#define k 1
#define m 1
#define g 9.8
#define n 0.5

#define h 1e-3
#define counter 10000

double t;

/* TODO[P0][P1]: 阻力应基于总速率 |v| 而非分量, 且不应乘 m, 详见文件头 */
void deriv(double t,double *y,double *dydt){
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
    double p[4];
    init(p,0,5,0,10);
    for(int i=0;i<counter;i++){
        step(p,h);
        if(p[2]<0) break;
        fprintf(fp,"%lf,%lf\n",p[0],p[2]);
    }
    return 0;
}
