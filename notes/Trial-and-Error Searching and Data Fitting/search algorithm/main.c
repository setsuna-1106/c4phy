#include "bisection.h"
#include <math.h>
#include <stdio.h>

#define V0 10.0  /* 势阱深度 */
#define EPS 1e-8 /* 搜索精度 */

/* cot(x) 在 math.h 中不直接提供，用 cos/sin */
static double cotan(double x) { return cos(x) / sin(x); }

/* ── 教材方程 (6.2): 偶宇称 ── */
double f_even(double EB) {
  double k = sqrt(V0 - EB);
  return k * tan(k) - sqrt(EB);
}

/* ── 教材方程 (6.3): 奇宇称 ── */
double f_odd(double EB) {
  double k = sqrt(V0 - EB);
  return k * cotan(k) - sqrt(EB);
}

/* ── 辅助：寻找区间内所有根（按步长扫描找变号区间） ── */
void find_roots(double (*f)(double), double start, double end, double step,
                const char *label) {
  double left = start, f_left = f(left);

  for (double right = left + step; right <= end; right += step) {
    double f_right = f(right);

    if (isnan(f_left) || isnan(f_right)) {
      left = right;
      f_left = f_right;
      continue;
    }

    if (f_left * f_right < 0) {
      double root = bisection(f, left, right, EPS);
      if (!isnan(root))
        printf("  %s EB = %12.8f  (区间 [%.4f, %.4f])\n", label, root, left,
               right);
    }
    left = right;
    f_left = f_right;
  }
}

int main(void) {
  printf("=== 1D 方势阱 V0=%.1f 束缚态能量 ===\n\n", V0);

  /* EB 的范围是 (0, V0)，tan/cot 在 k=n*π/2 处有无穷间断，
     用较小步长扫描确保不跳过根 */
  printf("偶宇称态:\n");
  find_roots(f_even, 0.001, V0 - 0.001, 0.01, "偶");

  printf("\n奇宇称态:\n");
  find_roots(f_odd, 0.001, V0 - 0.001, 0.01, "奇");

  return 0;
}
