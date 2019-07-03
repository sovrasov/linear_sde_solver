#define _USE_MATH_DEFINES
#include <limits>
#include <algorithm>
#include <cmath>
#include "mt_random.hpp"

MTRandom::MTRandom(int idx, size_t seed_group, size_t seed)
{
  static_assert(sizeof(unsigned) == 32 / 8, "");
  mGenerator = get_mt_parameter_id_st(32, 521, idx, seed_group);
  sgenrand_mt(seed, mGenerator);
}

double MTRandom::gen_uniform(double lb, double ub)
{
  unsigned rand_int = genrand_mt(mGenerator);
  return lb + (ub - lb) * static_cast<double>(rand_int) / std::numeric_limits<unsigned>::max();
}

double MTRandom::gen_normal(double mu, double sigma)
{
  double rand1 = -2 * log(std::max(std::numeric_limits<double>::min(), gen_uniform()));
  double rand2 = gen_uniform() * 2 * M_PI;
  return sqrt(sigma * rand1) * cos(rand2) + mu;
}

MTRandom::~MTRandom()
{
  if (mGenerator)
    free_mt_struct(mGenerator);
}

std::vector<std::unique_ptr<MTRandom>> MT2203Factory(size_t num_engines,
                                                     size_t group_seed,
                                                     std::vector<size_t> seeds)
{
  std::vector<std::unique_ptr<MTRandom>> generators;

  if (seeds.empty())
    for (size_t i = 0; i < num_engines; i++)
      seeds.push_back(group_seed + i);

  for (size_t i = 0; i < num_engines; i++)
    generators.push_back(std::make_unique<MTRandom>(i, group_seed, seeds[i]));

  return generators;
}
