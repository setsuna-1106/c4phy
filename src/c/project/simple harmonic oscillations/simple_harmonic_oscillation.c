#include <_stdlib.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define k 50
#define m 1
#define dt 1e-2

double x_1, x_2, v_1, v_2, a_1, a_2;

void init() {
  srand(time(NULL));
  x_1 = (double)(rand() % 10);
  v_1 = (double)(rand() % 20);
  a_1 = -k * x_1 / m;

  x_2 = x_1;
  v_2 = v_1;
  a_2 = a_1;
}

void step_Euler(double t) {
  a_1 = -k * x_1 / m;
  x_1 += v_1 * t;
  v_1 += a_1 * t;
}
// 显式Euler方法

void step_Euler_Cromer(double t) {
  a_2 = -k * x_2 / m;
  v_2 += a_2 * t;
  x_2 += v_2 * t;
}
// 半隐式Euler方法

int main() {
  FILE *fp1 = fopen("x-v1.csv", "w");
  FILE *fp2 = fopen("x-v2.csv", "w");
  init();
  for (int i = 0; i < 1000; i++) {
    fprintf(fp1, "%lf,%lf\n", x_1, v_1);
    fprintf(fp2, "%lf,%lf\n", x_2, v_2);
    step_Euler_Cromer(dt);
    step_Euler(dt);
  }
  fclose(fp1);
  fclose(fp2);

  return 0;
}
