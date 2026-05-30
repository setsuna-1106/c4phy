#include "newton.h"
#include "difference.h"
#include <math.h>

#define INTERVAL  1e-6    /* 数值求导的步长 */
#define MAX_ITER  100     /* 最大迭代次数 */

int newton_iters  = 0;
int newton_failed = 0;

double newton_search(double (*fp)(double), double a, double eps) {
  newton_iters  = 0;
  newton_failed = 0;

  while (fabs(fp(a)) > eps && newton_iters < MAX_ITER) {
    double k = cd(fp, a, INTERVAL);

    /* 导数为零：跳过此步（退化为无法改进） */
    if (fabs(k) < 1e-15) { newton_failed = 1; return NAN; }

    a -= fp(a) / k;
    newton_iters++;
  }

  if (newton_iters >= MAX_ITER) {
    newton_failed = 1;
    return NAN;
  }
  return a;
}
