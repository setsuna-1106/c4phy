#include "bisection.h"
#include "newton.h"
#include <math.h>
#include <stdio.h>

/* ── 目标函数 f(m) = m - tanh(m/t) ── */
static double g_t; /* 当前温度，由循环设置 */

static double f_mag(double m) { return m - tanh(m / g_t); }

/* ── 主程序 ── */
int main(void) {
  double prev_m = 1.0; /* 牛顿法的暖启动初值 */

  printf("  t       m_bisect  iter_b   m_newton  iter_n  failed  |f(m)|\n");
  printf("─────────────────────────────────────────────────────────────\n");

  /* CSV 输出 */
  FILE *csv = fopen("magnetization.csv", "w");
  fprintf(csv, "t,m_bisect,iter_bisect,m_newton,iter_newton,newton_failed\n");

  for (int i = 0; i <= 30; i++) {
    double t = 0.05 + i * 0.05;
    g_t = t;

    double mb = 0.0;
    int ib = 0;
    double mn = 0.0;
    int in = 0, nfail = 0;

    /* ── 二分法 ── */
    if (t < 1.0) {
      /* 区间 [0.001, 1.0]: f(0.001)<0, f(1.0)>0 */
      mb = bisection(f_mag, 0.001, 1.0, 1e-8);
      ib = bisection_iters;
    } else {
      mb = 0.0;
      ib = 0; /* t≥1 无非平凡解 */
    }

    /* ── 牛顿法 ── */
    if (t < 1.0) {
      mn = newton_search(f_mag, prev_m, 1e-8); /* 暖启动 */
      in = newton_iters;
      nfail = newton_failed;

      if (!nfail && !isnan(mn))
        prev_m = mn; /* 成功后更新暖启动值 */
      else
        prev_m = t; /* 失败则用 t 作为下个猜测 */
    } else {
      mn = 0.0;
      in = 0;
      nfail = 0;
    }

    double residual = fabs(f_mag(isnan(mn) ? mb : mn));
    /* 对 t≥1，m=0 是精确根，residual=0 */

    printf(" %5.2f   %9.6f  %4d   %9.6f  %4d     %d    %.2e\n", t, mb, ib, mn,
           in, nfail, residual);

    fprintf(csv, "%.2f,%.8f,%d,%.8f,%d,%d\n", t, isnan(mb) ? 0.0 : mb, ib,
            isnan(mn) ? 0.0 : mn, in, nfail);
  }

  fclose(csv);
  return 0;
}
