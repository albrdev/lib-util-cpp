#ifndef __UTIL_RANDOM__COMMON_HPP__
#define __UTIL_RANDOM__COMMON_HPP__

#include <cstddef>
#include <iterator>
#include <random>

namespace Util::Random
{
  template<typename TIterator, typename TRandomGenerator>
  TIterator RandomIterator(TIterator begin, TIterator end, TRandomGenerator& randomGenerator)
  {
    if(begin >= end)
    {
      return begin;
    }

    std::uniform_int_distribution<std::size_t> tmpDistributor(0u, std::distance(begin, end) - 1u);
    TIterator result = begin;
    std::advance(result, tmpDistributor(randomGenerator));
    return result;
  }
} // namespace Util::Random

#endif // __UTIL_RANDOM__COMMON_HPP__
