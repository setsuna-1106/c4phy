#include "../include/lagrange.h"
#include <stdio.h>

/* Table 6.1: 中子散射截面数据 */
#define NDATA 9
static const double E[NDATA] = {0, 25, 50, 75, 100, 125, 150, 175, 200};
static const double S[NDATA] = {10.6, 16.0, 45.0, 83.5, 52.8,
                                19.9, 10.8, 8.25, 4.7};

int main(void) {
  /* ── 1. 全局 9 点 Lagrange（教材练习 2）── */
  printf("=== 9-point global Lagrange (5 MeV steps) ===\n");
  printf("  E (MeV)   sigma (MB)\n");
  for (int e = 0; e <= 200; e += 5) {
    double sig = lagrange_interp(NDATA, E, S, (double)e);
    printf("  %3d        %10.4f\n", e, sig);
  }

  /* ── 2. 局部 3 点 Lagrange（教材练习 4）── */
  printf("\n=== 3-point local Lagrange (5 MeV steps) ===\n");
  printf("  E (MeV)   sigma (MB)\n");
  for (int e = 0; e <= 200; e += 5) {
    /* 在 E 数组中找到离目标最近的三个点的起始位置 */
    int start;
    if (e < E[1])
      start = 0; /* 左边界 */
    else if (e >= E[NDATA - 2])
      start = NDATA - 3; /* 右边界 */
    else {
      /* 找到第一个 ≥ e 的 E[i]，取 i-1 作为中心 */
      for (start = 0; start < NDATA - 2; start++)
        if (E[start + 1] >= e)
          break;
    }
    double sig = lagrange_interp(3, &E[start], &S[start], (double)e);
    printf("  %3d        %10.4f\n", e, sig);
  }

  return 0;
}
