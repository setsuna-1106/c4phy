#include <stdio.h>

int main(void)
{
    int n;
    double s_up = 0.0, s_down = 0.0;

    printf("input n\n");
    scanf("%d", &n);

    for (int i = 1; i <= n; i++) {
        s_up += 1.0 / i;
    }

    for (int i = n; i > 0; i--) {
        s_down += 1.0 / i;
    }

    printf("sum_up   = %.15lf\n", s_up);
    printf("sum_down = %.15lf\n", s_down);

    return 0;
}
/*
这里展示了两种计算调和级数的方法
第一种为先加大的数值，第二种为先加小的数值
由于double 的有效数字有限，所以先加小的数值的误差较小
*/