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
    double probabilitySum            = 0.0;
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
          probabilitySum += object->GetProbability();
        }
      }
      else if(object->GetProbability() < ProbabilityMin)
      {
        effectivePool.push_back(object);
        countMap[object] = 0u;
        autoProbabilityCount++;
      }
    }

    if(effectivePool.size() > 0u && autoProbabilityCount > 0u)
    {
      probabilitySum += static_cast<double>(autoProbabilityCount) / static_cast<double>(effectivePool.size());
    }

    if(count > guaranteedCount)
    {
      std::size_t effectiveCount = count - guaranteedCount;
      for(std::size_t i = 0u; i < effectiveCount; i++)
      {
        double hitValue = __randomDistributor(randomGenerator) * probabilitySum;

        double probabilityAdditive = 0.0;
        for(auto object : effectivePool)
        {
          probabilityAdditive += object->GetProbability() >= ProbabilityMin ? object->GetProbability() : (1.0 / static_cast<double>(effectivePool.size()));
          std::unordered_map<IRdsObject*, std::size_t>::iterator iter;
          if(probabilityAdditive > hitValue && (iter = countMap.find(object))->second < object->GetMaxCount() && callback(object))
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
