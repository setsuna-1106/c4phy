#include <stdio.h>
#include <math.h>

#include "../common/rk4_2d.h"

#define hbar 1
#define m 1
#define a 2
#define V0 83

#define h 1e-3      /* TODO(P1): 步长不足——势阱边缘 V 阶跃使 RK4 在间断点局部降阶,
                       实测浅态 κ 偏 +4.6e-3; 改 5e-4 即收敛到 ~1e-7 */
#define tolerance 1e-1   /* TODO(P0): 阈值式搜索无法定位本征值(见 main), 随搜索重构一并删除 */

double k;// eigenvalue
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

double mis_func(){
    double x1=-x0,x2=x0;
    double y1[2],y2[2];
    y1[0]=exp(k*x1);
    y1[1]=y1[0]*k;
    y2[0]=exp(k*x2);          /* TODO(P3): 振幅应为 exp(-k*x0) 与左侧对称, 现不影响对数导数但不规范 */
    y2[1]=-y2[0]*k;
    /* TODO(P1): while 循环浮点累计会多走/少走一步——实测左侧 12201 步(终点 2.201)、
       右侧 7800 步(终点 2.200), 两侧评估点错开 1e-3, 本征值整体偏移 ~5e-3;
       应改为每侧精确整数步数: 左 (x0+xm)/h=12200, 右 (x0-xm)/h=7800 (两侧步数不同!) */
    while(x1<xm){
        rk4_2d(deriv,y1,x1,h);
        x1+=h;
    }
    while(x2>xm){
        rk4_2d(deriv,y2,x2,h);   /* TODO(P1): 步长应为 -h: rk4_2d 从 t 积到 t+dt,
                                    当前状态实际向右推进却被记作向左推进 */
        x2-=h;
    }
    double ld_l,ld_r;
    ld_l=y1[1]/y1[0];
    ld_r=y2[1]/y2[0];
    /* TODO(P0): 对数导数形式有极点, 不可用——ψ_L(xm)=0 处 ld_l 发散:
       实测 κ≈2.138 处出伪根, 深态处极点与真根重合致 κ≈12.68/12.80/12.86 三个态全部漏掉;
       应改朗斯基行列式 return y1[1]*y2[0] - y1[0]*y2[1];
       (无除法无极点; W=0 <=> 两解线性相关 <=> 本征态) */
    return (ld_l-ld_r)/(ld_l+ld_r);
}

int main(){
    /* TODO(P0): 本征值搜索需整体重构:
       1) k 无物理上限(应为 κmax=√(2mV0)/ℏ≈12.884), 失配恒大时死循环;
       2) "递增直到失配<阈值"不是求根, 本征值对应失配函数的变号点;
       应改为: 在 (1e-3, κmax) 内以步长 ~5e-3 扫描, 发现变号即二分到 1e-9,
       逐个输出 κ 与 E=-κ²/2, 预期 17 个束缚态(可与解析超越方程 z·tan z=√(z0²-z²)、
       -z·cot z=√(z0²-z²), z0=a√(2mV0)/ℏ≈25.768 对照) */
    k=1;
    while(mis_func()>tolerance){
        k+=1e-1;
    }
    printf("%.2lf",k);   /* TODO(P2): 补 \n; 输出能量 E 而非仅 κ; 后续加波函数 CSV 供 Python 绘图 */
    return 0;
}