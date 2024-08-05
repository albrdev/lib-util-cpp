#include "SystemCall.hpp"
#include "exception/ErrnoError.hpp"

#include <iostream>
#include <string>

#include <gtest/gtest.h>

using namespace ::testing;

namespace UnitTest
{
  TEST(SystemCall, Basics)
  {
    try
    {
      Util::SystemCall exe("/home/albrdev/bin/bla", {"-a", "bla1", "-b", "bla2", "abc 123"}, {"BLA1=abc123", "BLA2=abc 123"});
      std::cout << "isActive2=" << exe.IsActive() << ", status=" << exe.GetStatus() << ", signal=" << exe.GetSignal() << std::endl;
      exe.Start();

      std::string result1 = exe.ReadStdout();
      std::cout << "MESSGAE1: '" << result1 << "' (" << result1.length() << ")" << std::endl;

      std::string result2 = exe.ReadStdout();
      std::cout << "MESSGAE2: '" << result2 << "' (" << result2.length() << ")" << std::endl;

      std::string msg = "BLA 12345";
      exe.WriteStdin(msg);
      std::cout << "isActive2=" << exe.IsActive() << ", status=" << exe.GetStatus() << ", signal=" << exe.GetSignal() << std::endl;

      std::string result3 = exe.ReadStderr();
      std::cout << "MESSGAE3: '" << result3 << "' (" << result3.length() << ")" << std::endl;

      bool isActive = exe.Await();
      std::cout << "isActive=" << isActive << ", isActive2=" << exe.IsActive() << ", status=" << exe.GetStatus() << ", signal=" << exe.GetSignal() << std::endl;
    }
    catch(const Util::Exception::ErrnoError& e)
    {
      std::cout << "*** Error: " << e.what() << std::endl;
    }
  }
} // namespace UnitTest
