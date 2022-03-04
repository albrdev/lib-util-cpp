#include <gtest/gtest.h>
#include "Error.hpp"

using namespace ::testing;

namespace UnitTest
{
  TEST(Error, Basics)
  {
    ASSERT_EQ(Util::Error::Get(), nullptr);

    Util::Error::Set<Util::Error::BoolError>(true);
    ASSERT_NE(Util::Error::Get(), nullptr);
    ASSERT_TRUE(*Util::Error::Get());
    ASSERT_EQ(Util::Error::Get()->ToString(), "");

    Util::Error::Set<Util::Error::BoolError>(false);
    ASSERT_NE(Util::Error::Get(), nullptr);
    ASSERT_FALSE(*Util::Error::Get());
    ASSERT_EQ(Util::Error::Get()->ToString(), "");

    Util::Error::Clear();
    ASSERT_EQ(Util::Error::Get(), nullptr);
  }
} // namespace UnitTest
