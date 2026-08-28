#ifndef DIFFERENCE_H
#define DIFFERENCE_H

double fd(double (*fp)(double), double x, double interl);
double cd(double (*fp)(double), double x, double interl);
double ed(double (*fp)(double), double x, double interl);
double double_derivate_cd_1(double (*fp)(double), double x, double interl);
double double_derivate_cd_2(double (*fp)(double), double x, double interl);

#endif
