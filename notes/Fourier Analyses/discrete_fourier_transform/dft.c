#include "dft.h"
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

void dft(const double *x, double ans[][2], int n) {
  for (int k = 0; k < n; k++) {
    double w = 2 * M_PI * k / n; /* 第 k 个 bin 的相位步进 */
    double re = 0, im = 0;
    for (int j = 0; j < n; j++) {
      re += x[j] * cos(j * w);
      im -= x[j] * sin(j * w);
    }
    ans[k][0] = re;
    ans[k][1] = im;
  }
}
