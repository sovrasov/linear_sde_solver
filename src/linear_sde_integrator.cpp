#include <cmath>

#include "linear_sde_integrator.hpp"

double EulerScheme::operator()(std::function<double(double)> f, double d, double h, double x, double y)
{
  return x + h*f(x) + sqrt(2*d*h)*y;
}

double HeunScheme::operator()(std::function<double(double)> f, double d, double h, double x, double y)
{
  double noise_offset = sqrt(2*d*h)*y;
  double f0 = f(x);
  double x1 = x + h*f0 + noise_offset;
  return x + noise_offset + 0.5*h*(f0 + f(x1));
}
