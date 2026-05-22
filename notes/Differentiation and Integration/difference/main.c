#include "difference.h"
#include <math.h>
#include <stdio.h>

#define H 1e5

int main() {
  FILE *fp = fopen("error.csv", "w");
  double x = 1;
  double exact = cos(x);
  for (int i = 1; i <= 10000; i++) {
    fprintf(
        fp, "%d,%.15lf,%.15lf,%.15lf\n", i, fd(sin, x, (double)i / H) - exact,
        cd(sin, x, (double)i / H) - exact, ed(sin, x, (double)i / H) - exact);
  }

  fclose(fp);
  return 0;
}
