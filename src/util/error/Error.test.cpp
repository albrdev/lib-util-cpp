#include "Error.hpp"

#include <gtest/gtest.h>

using namespace ::testing;

namespace UnitTest
{
  TEST(Error, Basics)
  {
    ASSERT_FALSE(Util::Error::Get());
    ASSERT_EQ(Util::Error::Get().ToString(), "");

    Util::Error::Set<Util::Error::BoolError>(true);
    ASSERT_TRUE(Util::Error::Get());
    ASSERT_EQ(Util::Error::Get().ToString(), "");

    Util::Error::Set<Util::Error::BoolError>(false);
    ASSERT_FALSE(Util::Error::Get());
    ASSERT_EQ(Util::Error::Get().ToString(), "");

    Util::Error::Set<Util::Error::IntError>();
    ASSERT_FALSE(Util::Error::Get());
    ASSERT_EQ(Util::Error::Get().ToString(), "");

    Util::Error::Set<Util::Error::IntError>(12345);
    ASSERT_TRUE(Util::Error::Get());
    ASSERT_EQ(Util::Error::Get().ToString(), "");

    Util::Error::Set<Util::Error::StringError>();
    ASSERT_FALSE(Util::Error::Get());
    ASSERT_EQ(Util::Error::Get().ToString(), "");

    Util::Error::Set<Util::Error::StringError>("Test");
    ASSERT_TRUE(Util::Error::Get());
    ASSERT_EQ(Util::Error::Get().ToString(), "Test");

    Util::Error::Clear();
    ASSERT_FALSE(Util::Error::Get());
    ASSERT_EQ(Util::Error::Get().ToString(), "");
  }
} // namespace UnitTest
