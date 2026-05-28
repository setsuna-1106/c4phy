#include "bisection.h"
#include <math.h>
#include <stdio.h>

double bisection(double (*fp)(double), double a, double b, double eps) {
  double plus, minus;

  /* 1. 确定正负端，同时处理端点恰好为根的情况 */
  double fa = fp(a), fb = fp(b);
  if (fabs(fa) < eps)
    return a;
  if (fabs(fb) < eps)
    return b;
  if (fa * fb > 0)
    return NAN; /* 同号，区间内无根 */

  if (fa > 0) {
    plus = a;
    minus = b;
  } else {
    plus = b;
    minus = a;
  }

  /* 2. 主循环：每次迭代开头只算一次中点 */
  while (fabs(plus - minus) > eps) {
    double mid = (plus + minus) * 0.5;
    double fm = fp(mid);

    if (fabs(fm) < eps)
      return mid;

    if (fm > 0)
      plus = mid;
    else
      minus = mid;
  }
  return (plus + minus) * 0.5;
}
