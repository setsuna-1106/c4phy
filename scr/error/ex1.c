#include <stdio.h>
#include <math.h>

int main (int argc,char **argv)
{   
    double x=0;
    printf("input x\n");
    scanf("%lf",&x);
    printf("f(x)=\\sqrt{x*x+1}-x = %.20lf\n",sqrt(x*x+1)-x);
    printf("f(x)=1/(\\sqrt{x*x+1}+x) = %.20lf",1/(sqrt(x*x+1)+x));
}
/*大数相减会导致结果不可靠性增加，可以将减法转化成加法*/