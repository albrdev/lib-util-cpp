#include "Rds.hpp"
#include "RdsObject.hpp"

#include <algorithm>
#include <exception>
#include <iostream>
#include <map>
#include <random>
#include <utility>
#include <vector>

#include <gtest/gtest.h>

using namespace ::testing;
using namespace Util::Random;
using Util::Random::Rds::IRdsObject;
using Util::Random::Rds::RdsObject;

namespace UnitTest
{
  static bool onAcquire(IRdsObject* object)
  {
    RdsObject<std::size_t>* pObject = dynamic_cast<RdsObject<std::size_t>*>(object);
    (*pObject)++;
    return true;
  }

  static void printResults(const std::map<std::string, RdsObject<std::size_t>>& objects, std::size_t count)
  {
    for(const auto& i : objects)
    {
      std::cout << i.first << ": " << std::setprecision(5) << ((static_cast<double>(i.second) / static_cast<double>(count)) * 100.0) << '%' << std::endl;
    }
  }

  static std::random_device randomDevice;
  static std::default_random_engine randomGenerator(randomDevice());

  TEST(RdsGenerator, Probabilities)
  {
    std::map<std::string, RdsObject<std::size_t>> cache {{"Low", RdsObject<std::size_t>(0u, 0.1)},
                                                         {"Medium", RdsObject<std::size_t>(0u, 0.5)},
                                                         {"High", RdsObject<std::size_t>(0u, 0.9)}};

    std::vector<IRdsObject*> objects;
    objects.reserve(cache.size());
    std::transform(cache.begin(), cache.end(), std::back_inserter(objects), [](auto& element) { return &element.second; });

    const std::size_t count = 1000u;
    Rds::Generate(objects, count, onAcquire, randomGenerator);

    printResults(cache, count);
  }

  TEST(RdsGenerator, AutoProbabilities)
  {
    std::map<std::string, RdsObject<std::size_t>> cache {{"Item1", RdsObject<std::size_t>(0u, Util::Random::Rds::ProbabilityDefault)},
                                                         {"Item2", RdsObject<std::size_t>(0u, Util::Random::Rds::ProbabilityDefault)},
                                                         {"Item3", RdsObject<std::size_t>(0u, Util::Random::Rds::ProbabilityDefault)},
                                                         {"Item4", RdsObject<std::size_t>(0u, Util::Random::Rds::ProbabilityDefault)}};

    std::vector<IRdsObject*> objects;
    objects.reserve(cache.size());
    std::transform(cache.begin(), cache.end(), std::back_inserter(objects), [](auto& element) { return &element.second; });

    const std::size_t count = 1000u;
    Rds::Generate(objects, count, onAcquire, randomGenerator);

    printResults(cache, count);
  }

  TEST(RdsGenerator, GuaranteedAndExcluded)
  {
    std::map<std::string, RdsObject<std::size_t>> cache {{"Excluded", RdsObject<std::size_t>(0u, 0.0)},
                                                         {"Low", RdsObject<std::size_t>(0u, 0.25)},
                                                         {"Medium", RdsObject<std::size_t>(0u, 0.5)},
                                                         {"High", RdsObject<std::size_t>(0u, 0.75)},
                                                         {"Guaranteed", RdsObject<std::size_t>(0u, 1.0, 1u)}};

    std::vector<IRdsObject*> objects;
    objects.reserve(cache.size());
    std::transform(cache.begin(), cache.end(), std::back_inserter(objects), [](auto& element) { return &element.second; });

    const std::size_t count = 1000u;
    Rds::Generate(objects, count, onAcquire, randomGenerator);

    printResults(cache, count);
    ASSERT_EQ(cache.at("Excluded"), 0u);
    ASSERT_EQ(cache.at("Guaranteed"), 1u);
  }

  TEST(RdsGenerator, SingleResult)
  {
    std::map<std::string, RdsObject<std::size_t>> cache {{"Low", RdsObject<std::size_t>(0u, 0.1)},
                                                         {"Medium", RdsObject<std::size_t>(0u, 0.5)},
                                                         {"High", RdsObject<std::size_t>(0u, 0.9)}};

    std::vector<IRdsObject*> objects;
    objects.reserve(cache.size());
    std::transform(cache.begin(), cache.end(), std::back_inserter(objects), [](auto& element) { return &element.second; });

    const std::size_t count = 1000u;
    Rds::Generate(objects, count, onAcquire, randomGenerator);

    printResults(cache, count);
  }

  TEST(RdsGenerator, CountedResults)
  {
    std::map<std::string, RdsObject<std::size_t>> cache {{"Item1", RdsObject<std::size_t>(0u, 0.05, 50u)},
                                                         {"Item2", RdsObject<std::size_t>(0u, 0.125, 125u)},
                                                         {"Item3", RdsObject<std::size_t>(0u, 0.375, 375u)},
                                                         {"Item4", RdsObject<std::size_t>(0u, 0.45, 450u)}};

    std::vector<IRdsObject*> objects;
    objects.reserve(cache.size());
    std::transform(cache.begin(), cache.end(), std::back_inserter(objects), [](auto& element) { return &element.second; });

    const std::size_t count = 1000u;
    Rds::Generate(objects, count, onAcquire, randomGenerator);

    printResults(cache, count);
    ASSERT_LE(cache.at("Item1"), 50u);
    ASSERT_LE(cache.at("Item2"), 125u);
    ASSERT_LE(cache.at("Item3"), 375u);
    ASSERT_LE(cache.at("Item4"), 450u);
  }

  TEST(RdsGenerator, CountedGuaranteedResults)
  {
    std::map<std::string, RdsObject<std::size_t>> cache {{"Item1", RdsObject<std::size_t>(0u, 1.0, 100u)},
                                                         {"Item2", RdsObject<std::size_t>(0u, 1.0, 200u)},
                                                         {"Item3", RdsObject<std::size_t>(0u, 1.0, 300u)},
                                                         {"Item4", RdsObject<std::size_t>(0u, 1.0, 400u)}};

    std::vector<IRdsObject*> objects;
    objects.reserve(cache.size());
    std::transform(cache.begin(), cache.end(), std::back_inserter(objects), [](auto& element) { return &element.second; });

    const std::size_t count = 1000u;
    Rds::Generate(objects, count, onAcquire, randomGenerator);

    printResults(cache, count);
    ASSERT_EQ(cache.at("Item1"), 100u);
    ASSERT_EQ(cache.at("Item2"), 200u);
    ASSERT_EQ(cache.at("Item3"), 300u);
    ASSERT_EQ(cache.at("Item4"), 400u);
  }

  TEST(RdsGenerator, LessCountedGuaranteedResults)
  {
    std::map<std::string, RdsObject<std::size_t>> cache {{"Item1", RdsObject<std::size_t>(0u, 1.0, 50u)},
                                                         {"Item2", RdsObject<std::size_t>(0u, 1.0, 100u)},
                                                         {"Item3", RdsObject<std::size_t>(0u, 1.0, 150u)},
                                                         {"Item4", RdsObject<std::size_t>(0u, 1.0, 200u)}};

    std::vector<IRdsObject*> objects;
    objects.reserve(cache.size());
    std::transform(cache.begin(), cache.end(), std::back_inserter(objects), [](auto& element) { return &element.second; });

    const std::size_t count = 1000u;
    Rds::Generate(objects, count, onAcquire, randomGenerator);

    printResults(cache, count);
    ASSERT_EQ(cache.at("Item1"), 50u);
    ASSERT_EQ(cache.at("Item2"), 100u);
    ASSERT_EQ(cache.at("Item3"), 150u);
    ASSERT_EQ(cache.at("Item4"), 200u);
  }

  TEST(RdsGenerator, InfiniteCountedObject)
  {
    ASSERT_THROW(RdsObject<std::size_t>(0u, Util::Random::Rds::ProbabilityMax, Util::Random::Rds::MaxCountLimit), std::length_error);
  }
} // namespace UnitTest
