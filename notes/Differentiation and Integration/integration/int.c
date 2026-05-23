#include "int.h"
#include <math.h>
#include <stdio.h>

double ti(double (*fp)(double), double i, double e, int N) {
  if (!fp || N <= 0) return NAN;
  if (i == e) return 0.0;
  double a = i, b = e;
  if (a > b) { a = e; b = i; }
  double h = (b - a) / N;
  double inte = 0;
  for (int j = 0; j <= N; j++) {
    if (j == 0 || j == N) {
      inte += h * fp(a + j * h) / 2;
    } else {
      inte += h * fp(a + j * h);
    }
  }
  return inte;
}

double si(double (*fp)(double), double i, double e, int N) {
  if (!fp || N <= 1) return NAN;
  if (i == e) return 0.0;
  double a = i, b = e;
  if (a > b) { a = e; b = i; }
  if (N % 2 == 1) {
    fprintf(stderr, "Simpson: N=%d odd → using N=%d\n", N, N + 1);
    N++;
  }
  double h = (b - a) / N;
  double inte = 0;

  for (int j = 0; j <= N; j++) {
    if (j == 0 || j == N) {
      inte += h * fp(a + j * h) / 3;
    } else {
      if (j % 2 == 1) {
        inte += 4 * h * fp(a + j * h) / 3;
      } else {
        inte += 2 * h * fp(a + j * h) / 3;
      }
    }
  }

  return inte;
}
