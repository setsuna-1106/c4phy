#include <stdio.h>
#include <math.h>
#include "../common/rk4_2d.h"

#define h 1e-5

double t;

/* TODO(P2): 输出角度应改用 atan2(x_[1], y_[1]) 直接给出散射角 theta,
 *   而不是只打印 tan 值; printf 末尾补换行, main 补 return */
/* TODO(P1): 核心缺口 —— 没有碰撞参数 b 扫描: 需循环多个 b (如 0~3 几百个点)
 *   记录 theta(b) 并写出数据文件, 才能看到 dtheta/db 的跳变 (混沌散射) */
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

void init(){
    t=0;

}

int main(){
    init();
    while(y_[0]<=20){
        step(h);
    }
    printf("the tan is %lf",x_[1]/y_[1]);
}