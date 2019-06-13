#pragma once

#include <functional>
#include <omp.h>

#include "mt_random.hpp"

template <typename T>
std::vector<std::vector<double>> integrate_linear_sde(size_t n_threads, size_t n_impls,
                            double step, size_t n_steps, double x0, double d,
                            std::function<double(double)> f)
{
  auto random_engines = MT2203Factory(n_threads, 123);
  std::vector<std::vector<double>> solutions;
  solutions.resize(n_impls);

#pragma omp parallel for num_threads(n_threads) schedule(static)
  for (size_t i = 0; i < n_impls; i++)
  {
    int thread_idx = omp_get_thread_num();
    double x = x0;
    solutions[i].resize(n_steps);
    for (size_t j = 0; j < n_steps; j++)
    {
      x = T()(f, d, step, x, random_engines[thread_idx]->gen_normal());
      solutions[i].push_back(x);
    }
  }

  return solutions;
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
