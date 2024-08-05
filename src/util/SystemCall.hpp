#ifndef __UTIL__SYSTEMCALL_HPP__
#define __UTIL__SYSTEMCALL_HPP__

#include <csignal>
#include <memory>
#include <string>
#include <vector>

#include <unistd.h>

namespace Util
{
  class SystemCall
  {
    public:
    static bool SendSignal(pid_t id, int signal = 0);

    pid_t GetPid() const;

    int GetExitStatus() const;
    int GetExitSignal() const;

    void WriteStdin(const std::string& message) const;
    std::string ReadStdout() const;
    std::string ReadStderr() const;

    bool IsActive();
    bool Start();
    bool Stop(bool force = false);
    void Suspend();
    void Resume();
    bool Await(int options = 0);
    bool Signal(int signal = 0);

    SystemCall(const std::string& filepath, const std::vector<std::string>& argumentList = {}, const std::vector<std::string>& environmentList = {});
    virtual ~SystemCall();

    private:
    bool _Await(int options);
    bool _AwaitExit(bool blocking);

    void SetupParent();
    void SetupChild();

    static void _CloseFileDescriptor(int& fileDescriptor);
    static void _CloseFileDescriptorPair(int (&fileDescriptors)[2]);
    void _CloseAllFileDescriptors();
    std::string _ReadOutputStream(int fileDescriptor) const;

    static void _GenerateRawArgumentList(const std::vector<std::string>& source, std::vector<std::unique_ptr<char[]>>& result);
    static void _GenerateRawArgumentList(const std::vector<std::unique_ptr<char[]>>& source, std::vector<char*>& result);

    const std::string& m_ExecutableFilepath;
    std::vector<std::unique_ptr<char[]>> m_ArgumentList;
    std::vector<std::unique_ptr<char[]>> m_EnvironmentList;

    pid_t m_Pid;
    int m_Status;

    int m_StdinFileDescriptor[2];
    int m_StdoutFileDescriptor[2];
    int m_StderrFileDescriptor[2];

    bool m_IsParent;
    bool m_IsRunning;
  };
} // namespace Util

#endif // __UTIL__SYSTEMCALL_HPP__
