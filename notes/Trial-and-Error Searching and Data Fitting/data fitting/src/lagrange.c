#include "lagrange.h"

double lagrange_interp(int n, const double x[], const double y[],
                       double x_target) {
    if (n <= 0) return 0.0;
    if (n == 1) return y[0];

    double result = 0.0;

    for (int i = 0; i < n; i++) {
        /* 若目标点恰好落在已知点上，直接返回，避免除零 */
        if (x_target == x[i]) return y[i];

        double lambda = 1.0;
        for (int j = 0; j < n; j++) {
            if (j == i) continue;
            lambda *= (x_target - x[j]) / (x[i] - x[j]);
        }
        result += y[i] * lambda;
    }

    return result;
}
