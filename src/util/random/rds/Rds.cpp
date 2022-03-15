#include "IRdsObject.hpp"
#include <random>

namespace Util::Random::Rds
{
  std::uniform_real_distribution<> __randomDistributor(ProbabilityMin, std::nextafter(ProbabilityMax, std::numeric_limits<double>::max()));
} // namespace Util::Random::Rds
