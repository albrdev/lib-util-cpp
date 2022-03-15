#include <gtest/gtest.h>
#include "Rds.hpp"
#include "RdsObject.hpp"
#include <vector>
#include <iostream>
#include <algorithm>
#include <random>
#include <utility>

using namespace ::testing;
using namespace Util::Random;
using Util::Random::Rds::IRdsObject;
using Util::Random::Rds::RdsObject;
using ElementType = std::pair<std::string, std::size_t>;

namespace UnitTest
{
  static bool onAcquire(IRdsObject* object)
  {
    RdsObject<ElementType>* pObject = dynamic_cast<RdsObject<ElementType>*>(object);
    pObject->GetObject().second++;
    return true;
  }

  static void printResults(const std::vector<RdsObject<ElementType>>& objects, std::size_t count)
  {
    for(const auto& i : objects)
    {
      std::cout << i.GetObject().first << ": " << std::setprecision(2) << ((static_cast<double>(i.GetObject().second) / static_cast<double>(count)) * 100.0)
                << '%' << std::endl;
    }
  }

  static std::random_device randomDevice;
  static std::default_random_engine randomGenerator(randomDevice());

  TEST(RdsGenerator, Basics)
  {
    std::vector<RdsObject<ElementType>> cache {
        RdsObject<ElementType>(std::make_pair<std::string, std::size_t>("Item1", 0u)),
        RdsObject<ElementType>(std::make_pair<std::string, std::size_t>("Item2", 0u)),
        RdsObject<ElementType>(std::make_pair<std::string, std::size_t>("Item3", 0u)),
        RdsObject<ElementType>(std::make_pair<std::string, std::size_t>("Item4", 0u)),
        RdsObject<ElementType>(std::make_pair<std::string, std::size_t>("Item5", 0u)),
    };

    std::vector<IRdsObject*> objects;
    objects.reserve(cache.size());
    std::transform(cache.begin(), cache.end(), std::back_inserter(objects), [](RdsObject<ElementType>& element) { return &element; });

    const std::size_t count = 10u;
    Rds::Generate(objects, count, onAcquire, randomGenerator);

    printResults(cache, count);
    ASSERT_TRUE(true);
  }

  TEST(RdsGenerator, Basics2)
  {
    std::vector<RdsObject<ElementType>> cache {
        RdsObject<ElementType>(std::make_pair<std::string, std::size_t>("Excluded", 0u), 0.0),
        RdsObject<ElementType>(std::make_pair<std::string, std::size_t>("Lowest", 0u), 0.25),
        RdsObject<ElementType>(std::make_pair<std::string, std::size_t>("Low", 0u), 0.5),
        RdsObject<ElementType>(std::make_pair<std::string, std::size_t>("High", 0u), 0.75),
        RdsObject<ElementType>(std::make_pair<std::string, std::size_t>("Guaranteed", 0u), 1.0, 1u),
    };

    std::vector<IRdsObject*> objects;
    objects.reserve(cache.size());
    std::transform(cache.begin(), cache.end(), std::back_inserter(objects), [](RdsObject<ElementType>& element) { return &element; });

    const std::size_t count = 10u;
    Rds::Generate(objects, count, onAcquire, randomGenerator);

    printResults(cache, count);
    ASSERT_TRUE(true);
  }

  TEST(RdsGenerator, Basics3)
  {
    std::vector<RdsObject<ElementType>> cache {RdsObject<ElementType>(std::make_pair<std::string, std::size_t>("Low", 0u), 0.1),
                                               RdsObject<ElementType>(std::make_pair<std::string, std::size_t>("Medium", 0u), 0.5),
                                               RdsObject<ElementType>(std::make_pair<std::string, std::size_t>("High", 0u), 0.9)};

    std::vector<IRdsObject*> objects;
    objects.reserve(cache.size());
    std::transform(cache.begin(), cache.end(), std::back_inserter(objects), [](RdsObject<ElementType>& element) { return &element; });

    const std::size_t count = 10u;
    Rds::Generate(objects, count, onAcquire, randomGenerator);

    printResults(cache, count);
    ASSERT_TRUE(true);
  }
} // namespace UnitTest
