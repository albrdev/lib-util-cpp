#include <gtest/gtest.h>
#include "Error.hpp"

using namespace ::testing;

namespace UnitTest
{
  TEST(Error, Basics)
  {
    ASSERT_EQ(Error::Get(), nullptr);

    Error::Set<Error::BoolError>(true);
    ASSERT_NE(Error::Get(), nullptr);
    ASSERT_TRUE(*Error::Get());
    ASSERT_EQ(Error::Get()->ToString(), "");

    Error::Set<Error::BoolError>(false);
    ASSERT_NE(Error::Get(), nullptr);
    ASSERT_FALSE(*Error::Get());
    ASSERT_EQ(Error::Get()->ToString(), "");

    Error::Clear();
    ASSERT_EQ(Error::Get(), nullptr);
  }
} // namespace UnitTest
