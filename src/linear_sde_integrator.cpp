#include "linear_sde_integrator.hpp"

double EulerScheme::operator()(std::function<double(double)> f, double x, double y)
{
  return x + h*f(x) + noise_koeff*y;
}

double HeunScheme::operator()(std::function<double(double)> f, double x, double y)
{
  double noise_offset = noise_koeff*y;
  double f0 = f(x);
  double x1 = x + h*f0 + noise_offset;
  return x + noise_offset + 0.5*h*(f0 + f(x1));
}
