#include "difference.h"

double fd(double (*fp)(double), double x, double interl) {
  return (fp(x + interl) - fp(x)) / interl;
}

double cd(double (*fp)(double), double x, double interl) {
  return (fp(x + interl) - fp(x - interl)) / (2 * interl);
}

double ed(double (*fp)(double), double x, double interl) {
  return (4 * cd(fp, x, interl/2) - cd(fp, x, interl)) / 3;
}

double double_derivate_cd_1(double (*fp)(double), double x, double interl) {
  return (cd(fp, x + interl / 2, interl / 2) -
          cd(fp, x - interl / 2, interl / 2)) /
         interl;
}

double double_derivate_cd_2(double (*fp)(double), double x, double interl) {
  return (fp(x + interl) + fp(interl) - 2 * fp(x)) / (interl * interl);
}
