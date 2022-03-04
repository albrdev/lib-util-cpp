#include <gtest/gtest.h>
#include "Version.hpp"

using namespace ::testing;
using namespace Util;

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
