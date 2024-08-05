#include "SystemCall.hpp"
#include "exception/ErrnoError.hpp"

#include <chrono>
#include <iostream>
#include <string>
#include <thread>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace ::testing;

namespace UnitTest
{
  TEST(SystemCall, Basic)
  {
    const std::string command = R"(exit 0)";

    Util::SystemCall exe("/usr/bin/env", {"sh", "-c", command});
    ASSERT_FALSE(exe.IsActive());
    ASSERT_EQ(exe.GetExitStatus(), 0);
    ASSERT_EQ(exe.GetExitSignal(), 0);

    ASSERT_NO_THROW(exe.Start());
    ASSERT_TRUE(exe.IsActive());
    ASSERT_EQ(exe.GetExitStatus(), 0);
    ASSERT_EQ(exe.GetExitSignal(), 0);

    bool awaitStatus;
    ASSERT_NO_THROW(awaitStatus = exe.Await());
    ASSERT_TRUE(awaitStatus);
    ASSERT_FALSE(exe.IsActive());
    ASSERT_EQ(exe.GetExitStatus(), 0);
    ASSERT_EQ(exe.GetExitSignal(), 0);
  }

  TEST(SystemCall, GetExitStatus)
  {
    const int exit_status     = 123;
    const std::string command = R"(sleep 0.1)"
                                "\n"
                                R"(exit )" +
                                std::to_string(exit_status);

    Util::SystemCall exe("/usr/bin/env", {"sh", "-c", command});
    ASSERT_FALSE(exe.IsActive());

    ASSERT_NO_THROW(exe.Start());
    ASSERT_TRUE(exe.IsActive());

    bool awaitStatus;
    ASSERT_NO_THROW(awaitStatus = exe.Await());
    ASSERT_TRUE(awaitStatus);
    ASSERT_FALSE(exe.IsActive());
    ASSERT_EQ(exe.GetExitStatus(), exit_status);
    ASSERT_EQ(exe.GetExitSignal(), 0);
  }

  TEST(SystemCall, GetExitSignal)
  {
    const int signal          = SIGTERM;
    const std::string command = R"(sleep 0.1)"
                                "\n"
                                R"(exit 0)";

    Util::SystemCall exe("/usr/bin/env", {"sh", "-c", command});
    ASSERT_FALSE(exe.IsActive());

    ASSERT_NO_THROW(exe.Start());
    ASSERT_TRUE(exe.IsActive());

    ASSERT_NO_THROW(exe.Signal(signal));

    bool awaitStatus;
    ASSERT_NO_THROW(awaitStatus = exe.Await());
    ASSERT_TRUE(awaitStatus);
    ASSERT_FALSE(exe.IsActive());
    ASSERT_EQ(exe.GetExitStatus(), 0);
    ASSERT_EQ(exe.GetExitSignal(), signal);
  }

  TEST(SystemCall, SuspendResume)
  {
    const unsigned long long int sleep_time = 100u;
    const std::string command               = R"(echo $(($(date "+%s%N") / 1000000)))"
                                              "\n"
                                              R"(sleep 0.01)"
                                              "\n"
                                              R"(echo $(($(date "+%s%N") / 1000000)))"
                                              "\n"
                                              R"(exit 0)";

    Util::SystemCall exe("/usr/bin/env", {"sh", "-c", command});
    ASSERT_FALSE(exe.IsActive());

    ASSERT_NO_THROW(exe.Start());
    ASSERT_TRUE(exe.IsActive());

    std::string output;
    ASSERT_NO_THROW(output = exe.ReadStdout());
    unsigned long long int start = std::stoull(output);
    ASSERT_NO_THROW(exe.Suspend());
    ASSERT_TRUE(exe.Await(WUNTRACED));
    ASSERT_TRUE(exe.IsActive());

    std::this_thread::sleep_for(std::chrono::milliseconds(sleep_time));
    ASSERT_TRUE(exe.IsActive());

    ASSERT_NO_THROW(exe.Resume());
    ASSERT_TRUE(exe.Await(WCONTINUED));
    ASSERT_TRUE(exe.IsActive());
    ASSERT_NO_THROW(output = exe.ReadStdout());
    unsigned long long int end = std::stoull(output);

    unsigned long long int diff = end - start;
    ASSERT_THAT(diff, AllOf(Ge(sleep_time), Lt(sleep_time + 10u)));

    bool awaitStatus;
    ASSERT_NO_THROW(awaitStatus = exe.Await());
    ASSERT_TRUE(awaitStatus);
    ASSERT_FALSE(exe.IsActive());
    ASSERT_EQ(exe.GetExitStatus(), 0);
    ASSERT_EQ(exe.GetExitSignal(), 0);
  }

  TEST(SystemCall, Read)
  {
    const std::string message = "abc 123";
    const std::string command = R"(echo )"
                                R"(")" +
                                message +
                                R"(")"
                                "\n"
                                R"(exit 0)";

    Util::SystemCall exe("/usr/bin/env", {"sh", "-c", command});
    ASSERT_FALSE(exe.IsActive());

    ASSERT_NO_THROW(exe.Start());
    ASSERT_TRUE(exe.IsActive());

    std::string output;
    ASSERT_NO_THROW(output = exe.ReadStdout());
    ASSERT_EQ(output, message);

    bool awaitStatus;
    ASSERT_NO_THROW(awaitStatus = exe.Await());
    ASSERT_TRUE(awaitStatus);
    ASSERT_FALSE(exe.IsActive());
    ASSERT_EQ(exe.GetExitStatus(), 0);
    ASSERT_EQ(exe.GetExitSignal(), 0);
  }

  TEST(SystemCall, Write)
  {
    const int exit_code       = 123;
    const std::string message = std::to_string(exit_code);
    const std::string command = R"(read INPUT)"
                                "\n"
                                R"(exit )" +
                                message;

    Util::SystemCall exe("/usr/bin/env", {"sh", "-c", command});
    ASSERT_FALSE(exe.IsActive());

    ASSERT_NO_THROW(exe.Start());
    ASSERT_TRUE(exe.IsActive());

    ASSERT_NO_THROW(exe.WriteStdin(message));

    bool awaitStatus;
    ASSERT_NO_THROW(awaitStatus = exe.Await());
    ASSERT_TRUE(awaitStatus);
    ASSERT_FALSE(exe.IsActive());
    ASSERT_EQ(exe.GetExitStatus(), exit_code);
    ASSERT_EQ(exe.GetExitSignal(), 0);
  }

  TEST(SystemCall, Full)
  {
    const std::string command = R"(echo "${0}")"
                                "\n"
                                R"(echo "${1}")"
                                "\n"
                                R"(echo "${ENV1}")"
                                "\n"
                                R"(echo "${ENV2}")"
                                "\n"
                                R"(read INPUT)"
                                "\n"
                                R"(>&2 echo "${INPUT}")"
                                "\n"
                                R"(exit 0)";

    const std::string arg1  = "--arg1";
    const std::string arg2  = "--arg2 123";
    const std::string env1  = "abc";
    const std::string env2  = "abc 123";
    const std::string input = "abc 123";

    Util::SystemCall exe("/usr/bin/env", {"sh", "-c", command, arg1, arg2}, {"ENV1=" + env1, "ENV2=" + env2});
    ASSERT_FALSE(exe.IsActive());

    ASSERT_NO_THROW(exe.Start());
    ASSERT_TRUE(exe.IsActive());

    std::string output;
    ASSERT_NO_THROW(output = exe.ReadStdout());
    ASSERT_EQ(output, arg1);
    ASSERT_NO_THROW(output = exe.ReadStdout());
    ASSERT_EQ(output, arg2);

    ASSERT_NO_THROW(output = exe.ReadStdout());
    ASSERT_EQ(output, env1);
    ASSERT_NO_THROW(output = exe.ReadStdout());
    ASSERT_EQ(output, env2);

    ASSERT_NO_THROW(exe.WriteStdin(input));
    ASSERT_NO_THROW(output = exe.ReadStderr());
    ASSERT_EQ(output, input);

    bool awaitStatus;
    ASSERT_NO_THROW(awaitStatus = exe.Await());
    ASSERT_TRUE(awaitStatus);
    ASSERT_FALSE(exe.IsActive());
    ASSERT_EQ(exe.GetExitStatus(), 0);
    ASSERT_EQ(exe.GetExitSignal(), 0);
  }
} // namespace UnitTest
