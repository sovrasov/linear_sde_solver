#pragma once

#include <functional>
#include <omp.h>

#include "mt_random.hpp"

template <typename T>
double integrate_linear_sde(size_t n_threads, size_t n_impls,
                            double step, double max_time, double d,
                            std::function<double(double)> f, double x0)
{
  auto random_engines = MT2203Factory(n_threads, 123);
  size_t n_steps = static_cast<size_t>(max_time / step);
  std::vector<double> solutions;
  solutions.reserve(n_steps);

#pragma omp parallel for num_threads(n_threads)
  for (size_t i = 0; i < n_impls; i++)
  {
    int thread_idx = omp_get_thread_num();
    double x = x0;
    for (size_t j = 0; j < n_steps; i++)
      x = T(f, d, step, x, random_engines[thread_idx]->gen_normal());
  }

  return 0;
}


class EulerScheme
{
public:
  double operator()(std::function<double(double)> f, double d, double h, double x, double y);
};

class HeunScheme
{
public:
  double operator()(std::function<double(double)> f, double d, double h, double x, double y);
};
