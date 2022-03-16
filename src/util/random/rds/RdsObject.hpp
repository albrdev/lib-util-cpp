#ifndef __UTIL_RANDOM_RDS__RDSOBJECT_HPP__
#define __UTIL_RANDOM_RDS__RDSOBJECT_HPP__

#include <algorithm>
#include <exception>
#include "IRdsObject.hpp"

namespace Util::Random::Rds
{
  template<class T>
  class RdsObject : public IRdsObject
  {
    public:
    operator const T&() const { return m_Object; }
    operator T&() { return m_Object; }

    virtual double GetProbability() const override { return m_Probability; }
    virtual std::size_t GetMaxCount() const override { return m_MaxCount; }

    RdsObject(const T& object, double probability = ProbabilityDefault, std::size_t maxCount = MaxCountLimit)
        : IRdsObject()
        , m_Object(object)
        , m_Probability(probability < ProbabilityMin ? ProbabilityDefault : (std::min(std::max(probability, ProbabilityMin), ProbabilityMax)))
        , m_MaxCount(maxCount)
    {
      if(m_Probability >= 1.0 && m_MaxCount == MaxCountLimit)
      {
        throw std::length_error("Infinite count specified for guaranteed object");
      }
    }

    RdsObject(const RdsObject& other)
        : IRdsObject()
        , m_Object(other.m_Object)
        , m_Probability(other.m_Probability)
        , m_MaxCount(other.m_MaxCount)
    {}

    RdsObject(RdsObject&& other)
        : IRdsObject()
        , m_Object(std::move(other.m_Object))
        , m_Probability(std::move(other.m_Probability))
        , m_MaxCount(std::move(other.m_MaxCount))
    {}

    RdsObject& operator=(const RdsObject& other)
    {
      m_Object      = other.m_Object;
      m_Probability = other.m_Probability;
      m_MaxCount    = other.m_MaxCount;
      return *this;
    }

    RdsObject& operator=(RdsObject&& other)
    {
      m_Object      = std::move(other.m_Object);
      m_Probability = std::move(other.m_Probability);
      m_MaxCount    = std::move(other.m_MaxCount);
      return *this;
    }

    virtual ~RdsObject() override = default;

    private:
    T m_Object;
    double m_Probability;
    std::size_t m_MaxCount;
  };
} // namespace Util::Random::Rds

#endif // __UTIL_RANDOM_RDS__RDSOBJECT_HPP__
