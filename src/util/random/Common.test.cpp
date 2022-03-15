#include <gtest/gtest.h>
#include "Common.hpp"
#include <vector>
#include <iostream>
#include <random>

using namespace ::testing;

namespace UnitTest
{
  static std::random_device randomDevice;
  static std::default_random_engine randomGenerator(randomDevice());

  TEST(Common, Basics)
  {
    const std::vector<int> values {0, 1, 2, 3, 4};

    for(std::size_t i = 0u; i < 10u; i++)
    {
      auto iterator = Util::Random::RandomIterator(values.begin(), values.end(), randomGenerator);
      std::cout << *iterator << std::endl;
    }

    ASSERT_TRUE(true);
  }
} // namespace UnitTest
