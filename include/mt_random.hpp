#pragma once

#include <vector>
#include <memory>

#include <dc.h>

class MTRandom
{
protected:
  mt_struct *mGenerator = nullptr;
public:
  MTRandom(int idx, size_t seed_group, size_t seed);
  ~MTRandom();

  double gen_uniform(double lb=0., double ub=1.);
  double gen_normal(double mu=0., double sigma=1.);
};

std::vector<std::unique_ptr<MTRandom>> MT2203Factory(size_t num_engines,
                                                     size_t group_seed,
                                                     std::vector<size_t> seeds);
