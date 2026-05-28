#include "newton.h"
#include "difference.h"
#include <math.h>

#define interval 1e-6

double newton_search(double (*fp)(double), double a, double exp) {
  while (fabs(fp(a)) > exp) {
    double k = cd(fp, a, interval);
    a -= fp(a) / k;
  }
  return a;
}
