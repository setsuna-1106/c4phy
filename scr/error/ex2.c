#include <stdio.h>
#include <math.h>


int main (int argc,char **argv)
{
    int n=0;
    double s1=0,s2=0,s3=0;
    printf("input the n");
    scanf("%d",&n);
    for(int i=1;i<=2*n;i++){
        if(i%2) s1-=(double)i/(i+1);
        else s1+=(double)i/(i+1);
    }
    double s21=0,s22=0;
    for(int i=1;i<=n;i++){
        s21+=(double)(2*i-1)/(2*i);
        s22+=(double)(2*i)/(2*i+1);
    }
    s2=s22-s21;
    for(int i=1;i<=n;i++){
        s3 += 1.0 / ((double)(2*i) * (2*i + 1));
    }
    printf("%.10lf\n%.10lf\n%.10lf",s1,s2,s3);
}
/*
三种算法都在计算同一个求和
s1每两项之间相近，相减会产生误差
s2先求和后再相减，在n很大时，会出现大数相减，出现较大误差
s3每一项都是加法，误差较小
一般 s3>s1>s2
*/