#include "Version.hpp"

#include <gtest/gtest.h>

using namespace ::testing;
using namespace Util::Version;

namespace UnitTest
{
  TEST(Version, Empty)
  {
    Version version;

    ASSERT_EQ(version.GetMajor(), 0u);
    ASSERT_EQ(version.GetMinor(), 0u);
    ASSERT_EQ(version.GetBuild(), 0u);
  }
} // namespace UnitTest
