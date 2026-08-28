#include "newton.h"
#include <math.h>
#include <stdio.h>

int main() {
  double x = newton_search(sin, 2, 1e-6);
  printf("%lf", x);
  return 0;
}
