#include <stdio.h>
#include <math.h>

int main(void)
{
    double x, term, sum;
    double eps = 1e-8;
    int n = 1;

    printf("input x to calculate sin(x)\n");
    scanf("%lf", &x);

    term = x;
    sum = x;

    do {
        term = -term * x * x /(double)((2 * n) * (2 * n + 1));
        sum += term;
        n++;
    } while (fabs(term) > eps);

    printf("sin(x) = %.10lf\n", sum);
    printf("math.h sin(x) = %.10lf\n", sin(x));

    return 0;
}
/*
使用泰勒展开计算sinx的近似值
可以注意到在x较大时，例如50、100等数，会出先较大误差
这是因为x较大时，x的次方项数值极大，但是double 的有限数值有限
所以会产生较大的浮点误差
可以利用sinx的周期性来将较大的x对应到较小区间内的值
*/