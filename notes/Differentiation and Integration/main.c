#include "difference.h"
#include <math.h>
#include <stdio.h>

int main() {
  double x = 1;
  double exact = cos(x);

  double error_1 = fd(sin, 1, 1e-4) - exact;
  double error_2 = cd(sin, 1, 1e-4) - exact;
  double error_3 = ed(sin, 1, 1e-4) - exact;
  printf("%.15lf,%.15lf,%.15lf", error_1, error_2, error_3);
  return 0;
}
