#include <iostream>
#include <chrono>
#include <cmath>

#include <linear_sde_integrator.hpp>

int main(int argc, char const *argv[])
{
  auto start = std::chrono::system_clock::now();
  auto solutions = integrate_linear_sde<HeunScheme>(1, 1000, 0.001, 100000, 0, 1,
                                        [](double x){return cos(x);});

  auto end = std::chrono::system_clock::now();
  std::chrono::duration<double> elapsed_seconds = end - start;
  std::cout << "Time elapsed: " << elapsed_seconds.count() << "s\n";
  return 0;
}
