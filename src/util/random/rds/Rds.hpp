#ifndef __UTIL_RANDOM_RDS__RDS_HPP__
#define __UTIL_RANDOM_RDS__RDS_HPP__

#include <vector>
#include <unordered_map>
#include <functional>
#include <random>
#include "IRdsObject.hpp"

namespace Util::Random::Rds
{
  using RdsCallback = std::function<bool(IRdsObject*)>;

  extern std::uniform_real_distribution<> __randomDistributor;

  template<class T>
  void Generate(const std::vector<IRdsObject*>& pool, std::size_t count, const RdsCallback& callback, T& randomGenerator)
  {
    std::vector<IRdsObject*> effectivePool;
    std::unordered_map<IRdsObject*, std::size_t> countMap;
    double totalProbabilitySum       = 0.0;
    std::size_t guaranteedCount      = 0;
    std::size_t autoProbabilityCount = 0;

    for(auto object : pool)
    {
      if(object->GetProbability() > ProbabilityMin)
      {
        if(object->GetProbability() >= ProbabilityMax)
        {
          for(std::size_t i = 0u; i < object->GetMaxCount(); i++)
          {
            if(callback(object))
            {
              guaranteedCount++;
            }
          }
        }
        else
        {
          effectivePool.push_back(object);
          countMap[object] = 0u;
          totalProbabilitySum += object->GetProbability();
        }
      }
      else if(object->GetProbability() < ProbabilityMin)
      {
        effectivePool.push_back(object);
        countMap[object] = 0u;
        autoProbabilityCount++;
      }
    }

    if(effectivePool.size() == 0u)
    {
      return;
    }

    if(autoProbabilityCount > 0u)
    {
      totalProbabilitySum += static_cast<double>(autoProbabilityCount) / static_cast<double>(effectivePool.size());
    }

    const double autoProbability = 1.0 / static_cast<double>(effectivePool.size());
    if(count > guaranteedCount)
    {
      std::size_t effectiveCount = count - guaranteedCount;
      for(std::size_t i = 0u; i < effectiveCount; i++)
      {
        const double hitValue = __randomDistributor(randomGenerator) * totalProbabilitySum;

        double probabilitySum = 0.0;
        for(auto object : effectivePool)
        {
          probabilitySum += (object->GetProbability() >= ProbabilityMin) ? object->GetProbability() : autoProbability;
          std::unordered_map<IRdsObject*, std::size_t>::iterator iter;
          if(probabilitySum > hitValue && (iter = countMap.find(object))->second < object->GetMaxCount() && callback(object))
          {
            iter->second++;
            break;
          }
        }
      }
    }
  }
} // namespace Util::Random::Rds

#endif // __UTIL_RANDOM_RDS__RDS_HPP__
