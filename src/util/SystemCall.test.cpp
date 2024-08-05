#include "SystemCall.hpp"

#include <cerrno>
#include <iostream>
#include <string>

#include <gtest/gtest.h>

using namespace ::testing;

namespace UnitTest
{
  TEST(SystemCall, Basics)
  {
    Util::SystemCall exe("/home/albrdev/bin/bla", {"-a", "bla1", "-b", "bla2", "abc 123"}, {"BLA1=abc123", "BLA2=abc 123"});
    std::cout << "isActive2=" << exe.IsActive() << ", status=" << exe.GetStatus() << ", signal=" << exe.GetSignal() << std::endl;
    exe.Start();

    std::string result1;
    if(!exe.ReadStdout(result1))
    {
      FAIL() << "*** Error: read(): " << strerror(errno) << std::endl;
    }
    std::cout << "MESSGAE1: '" << result1 << "' (" << result1.length() << ")" << std::endl;

    std::string result2;
    if(!exe.ReadStdout(result2))
    {
      FAIL() << "*** Error: read(): " << strerror(errno) << std::endl;
    }
    std::cout << "MESSGAE1: '" << result2 << "' (" << result2.length() << ")" << std::endl;

    std::string msg = "BLA 12345";
    if(!exe.WriteStdin(msg))
    {
      FAIL() << "*** Error: write(): " << strerror(errno) << std::endl;
    }

    std::cout << "isActive2=" << exe.IsActive() << ", status=" << exe.GetStatus() << ", signal=" << exe.GetSignal() << std::endl;

    std::string result3;
    if(!exe.ReadStderr(result3))
    {
      FAIL() << "*** Error: read(): " << strerror(errno) << std::endl;
    }
    std::cout << "MESSGAE2: '" << result3 << "' (" << result3.length() << ")" << std::endl;

    bool isActive = exe.Await();
    std::cout << "isActive=" << isActive << ", isActive2=" << exe.IsActive() << ", status=" << exe.GetStatus() << ", signal=" << exe.GetSignal() << std::endl;
  }
} // namespace UnitTest
