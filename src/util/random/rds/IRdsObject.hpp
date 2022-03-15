#ifndef __UTIL_RANDOM_RDS__IRDSOBJECT_HPP__
#define __UTIL_RANDOM_RDS__IRDSOBJECT_HPP__

#include <cstddef>
#include <limits>

namespace Util::Random::Rds
{
  inline constexpr double ProbabilityDefault = -1.0;
  inline constexpr double ProbabilityMin     = 0.0;
  inline constexpr double ProbabilityMax     = 1.0;

  inline constexpr std::size_t MaxCountLimit = std::numeric_limits<std::size_t>::max();

  class IRdsObject
  {
    public:
    virtual double GetProbability() const   = 0;
    virtual std::size_t GetMaxCount() const = 0;

    virtual ~IRdsObject() = default;

    protected:
    IRdsObject() = default;

    private:
    IRdsObject(const IRdsObject&) = delete;
    IRdsObject& operator=(const IRdsObject&) = delete;
  };
} // namespace Util::Random::Rds

#endif // __UTIL_RANDOM_RDS__IRDSOBJECT_HPP__
