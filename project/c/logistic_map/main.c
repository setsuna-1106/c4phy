/*
 * TODO(按优先级评级):
 * [P0] mu=2 硬编码, 目前只演示了收敛到不动点 x*=1-1/mu=0.5 的定态行为;
 *      应改为扫描 mu∈[2.5,4.0], 每个 mu 先丢弃瞬态(如 500 步)再采样若干 x,
 *      输出 (mu,x) 分岔图数据 —— 倍周期分岔与混沌才是本主题的核心.
 *
 * [P1] 项目缺 Makefile(产物应进 build/), 且未在根 Makefile 注册,
 *      make / make c 均不会构建本项目.
 *
 * [P1] 缺 Python 可视化配套: 按仓库 C→CSV→matplotlib 工作流,
 *      建 project/python/logistic_map/ 绘制分岔图.
 *
 * [P2] README 项目结构树与学习进度表尚无 logistic_map 条目, 随上述工作补登;
 *      (可选) 补 document.md 记录模型与运行方式.
 *
 * [P3] 进阶: 计算 Lyapunov 指数 lambda=<ln|mu*(1-2x)|>, 与分岔图对照,
 *      验证混沌区 lambda>0.
 */

#include <stdio.h>

#define mu 3.7
#define x0 0.3

int main(){
    double x;
    FILE *fp=fopen("x.csv","w");
    if(fp==NULL){
        printf("file dont open");
        return 1;
    }

    x=x0;

    for(int i=0;i<1000;i++){
        x=mu*x*(1-x);
        fprintf(fp,"%d,%lf\n",i,x);
    }
    
    fclose(fp);

    return 0;
}