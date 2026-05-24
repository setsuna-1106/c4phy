#include "int.h"
#include <math.h>
#include <stdio.h>

int main() {
  double i = 0, f = 1;
  double exact = cos(0) - cos(1);

  FILE *fp = fopen("error.csv", "w");
  // for (int j = 1e3; j <= 1e4; j++) {
  //   fprintf(fp, "%d,%.16lf,%.16lf\n", 2 * j,
  //           (ti(sin, i, f, 2 * j) - exact) / exact,
  //           (si(sin, i, f, 2 * j) - exact) / exact);
  // }
  for (int j = 1e6; j <= 1e8; j += 5e5) {
    fprintf(fp, "%d,%.16lf\n", 2 * j, (ti(sin, i, f, 2 * j) - exact) / exact);
  }

  fclose(fp);
  return 0;
}
