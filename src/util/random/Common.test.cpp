#include <gtest/gtest.h>
#include "Common.hpp"
#include <vector>
#include <random>

using namespace ::testing;

namespace UnitTest
{
  static std::random_device randomDevice;
  static std::default_random_engine randomGenerator(randomDevice());

  TEST(Common, RandomIterator)
  {
    const std::vector<int> values {0, 1, 2, 3, 4};

    for(std::size_t i = 0u; i < 1000u; i++)
    {
      auto iterator = Util::Random::RandomIterator(values.cbegin(), values.cend(), randomGenerator);
      ASSERT_GE(iterator, values.begin());
      ASSERT_LT(iterator, values.cend());
    }
  }

  TEST(Common, RandomIteratorWithInvalidParameters)
  {
    const std::vector<int> values {0, 1, 2, 3, 4};
    auto iterator = Util::Random::RandomIterator(values.cend(), values.cbegin(), randomGenerator);
    ASSERT_EQ(iterator, values.cend());
  }

  TEST(Common, RandomIteratorWithEmptyContainer)
  {
    const std::vector<int> values {};
    auto iterator = Util::Random::RandomIterator(values.cbegin(), values.cend(), randomGenerator);
    ASSERT_EQ(iterator, values.cend());
  }
} // namespace UnitTest
