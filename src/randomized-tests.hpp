#pragma once
#include <random>
#include "types.hpp"

namespace cest
{
  void defaultRandomFn(int seed, cest::TestSuite *suite)
  {
    auto engine = std::default_random_engine(seed);
    std::shuffle(suite->test_cases.begin(), suite->test_cases.end(), engine);
  }

  void randomizeTests(TestSuite *suite, int seed, std::function<void(int, cest::TestSuite *)> random_fn)
  {
    random_fn(seed, suite);

    for (auto &pair : suite->test_suites)
      randomizeTests(pair.second, seed, random_fn);
  }
}