#include <stdio.h>
#include <math.h>
#include "../common/rk4_2d.h"

#define h 1e-5

/* TODO(P1): 重构积分器 —— x/y 耦合却拆成两次独立 rk4_2d + 全局变量传态, 脆弱且难扩展;
 *   应合并为 4 个一阶方程 (x, vx, y, vy) 单次积分, 届时可删除全部全局变量 */
double x_[2],y_[2];
double t;

/* TODO(P2): 输出角度应改用 atan2(x_[1], y_[1]) 直接给出散射角 theta,
 *   而不是只打印 tan 值; printf 末尾补换行, main 补 return */
/* TODO(P1): 核心缺口 —— 没有碰撞参数 b 扫描: 需循环多个 b (如 0~3 几百个点)
 *   记录 theta(b) 并写出数据文件, 才能看到 dtheta/db 的跳变 (混沌散射) */
/* TODO(P1): theta(b) 得到后计算微分截面 sigma(theta) = |dtheta/db| * b/sin(theta)
 *   (见笔记 13.3 节), 目前完全未实现 */
/* TODO(P3): 积分区间 y=±20 与固定步长 h=1e-5 可接受, 但可考虑靠近势阱区用自适应步长 */

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