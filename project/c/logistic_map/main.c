/*
 * TODO(按优先级评级):
 * [P0] 目前 mu=3 固定, N=3 条相邻初值轨道仅演示初值差异的衰减;
 *      核心目标是扫描 mu∈[2.5,4.0], 每个 mu 先丢弃瞬态(如 500 步)再采样若干 x,
 *      输出 (mu,x) 分岔图数据, Python 端改为绘制分岔图
 *      —— 倍周期分岔与混沌才是本主题的核心.
 *
 * [P2] (可选) 补 document.md 记录模型与运行方式.
 *
 * [P3] 进阶: 计算 Lyapunov 指数 lambda=<ln|mu*(1-2x)|>, 与分岔图对照,
 *      验证混沌区 lambda>0.
 */

#include <stdio.h>

#define mu 3
#define x0 0.3
#define N 3

int main(){
    double x[N];
    FILE *fp=fopen("x.csv","w");
    if(fp==NULL){
        printf("file dont open");
        return 1;
    }
    for(int i=0;i<N;i++){
        x[i]=x0+1e-2*i;
    }

    for(int i=0;i<1000;i++){
        fprintf(fp,"%d",i);
        for(int j=0;j<N;j++){
            x[j]=mu*x[j]*(1-x[j]);
            fprintf(fp,",%lf",x[j]);
        }
        fprintf(fp,"\n");
    }
    
    fclose(fp);

    return 0;
}