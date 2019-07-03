#pragma once

#include <functional>
#include <cmath>
#include <omp.h>
#include <chrono>

#include "mt_random.hpp"

template <typename T>
std::pair<std::vector<std::vector<double>>, double>
                            integrate_linear_sde(size_t n_threads, size_t n_impls,
                            double step, size_t n_steps, double x0, double d,
                            std::function<double(double)> f, size_t seed = 123)
{
  auto random_engines = MT2203Factory(n_threads, seed);
  std::vector<std::vector<double>> solutions;
  solutions.resize(n_impls);
  auto solver_rule = T(d, step);

  auto start = std::chrono::system_clock::now();
#pragma omp parallel for num_threads(n_threads) schedule(static)
  for (int i = 0; i < static_cast<int>(n_impls); i++)
  {
    int thread_idx = omp_get_thread_num();
    double x = x0;
    solutions[i].reserve(n_steps);
    for (size_t j = 0; j < n_steps; j++)
    {
      x = solver_rule(f, x, random_engines[thread_idx]->gen_normal());
      solutions[i].push_back(x);
    }
  }
  auto end = std::chrono::system_clock::now();
  std::chrono::duration<double> elapsed_seconds = end - start;

  return std::make_pair(solutions, elapsed_seconds.count());
}

class Scheme
{
protected:
  double noise_koeff;
  double h;
public:
  Scheme(double d_, double h_) : noise_koeff(sqrt(2*d_*h_)), h(h_) {}
};

class EulerScheme : public Scheme
{
public:
  EulerScheme(double d, double h) : Scheme(d, h) {}
  double operator()(std::function<double(double)> f, double x, double y);
};

class HeunScheme : public Scheme
{
public:
  HeunScheme(double d, double h) : Scheme(d, h) {}
  double operator()(std::function<double(double)> f, double x, double y);
};
