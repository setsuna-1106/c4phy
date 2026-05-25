#include <_stdlib.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define k 50
#define m 1
#define nu 1
#define dt 1e-2

double x, v, a;
double t;

void init() {
  srand(time(NULL));
  x = (double)(rand() % 10);
  v = (double)(rand() % 20);
  a = -k * x / m;
  t = 0;
}

void step_Euler_Cromer(double t1) {
  a = -k * x / m - v * m;
  v += a * t1;
  x += v * t1;
  t += t1;
}
// 半隐式Euler方法

int main() {
  init();
  FILE *fp = fopen("x-t.csv", "w");
  for (int i = 0; i < 2000; i++) {
    step_Euler_Cromer(dt);
    fprintf(fp, "%lf,%lf\n", t, x);
  }
  fclose(fp);
  return 0;
}
