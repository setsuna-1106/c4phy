#ifndef LAGRANGE_H
#define LAGRANGE_H

/* n-point Lagrange interpolation.
 *   n        : number of data points to use
 *   x[], y[] : known data (x_i, y_i), i = 0..n-1
 *   x_target : the x value at which to interpolate
 * Returns the interpolated y at x_target.
 *
 * Note: x_target should lie within the range of x[].
 *       Extrapolation is allowed but unreliable. */
double lagrange_interp(int n, const double x[], const double y[],
                       double x_target);

#endif
