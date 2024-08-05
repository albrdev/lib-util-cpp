#include "SystemCall.hpp"
#include "exception/ErrnoError.hpp"

#include <algorithm>
#include <cerrno>
#include <cstdio>
#include <cstdlib>

#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>

#define READ_FD  0
#define WRITE_FD 1

namespace Util
{
  bool SystemCall::SendSignal(pid_t pid, int signal)
  {
    if(::kill(pid, signal) == -1)
    {
      if(errno == EINVAL)
      {
        throw Exception::ErrnoError("kill()");
      }

      return false;
    }

    return true;
  }

  pid_t SystemCall::GetPid() const { return m_Pid; }

  int SystemCall::GetExitStatus() const { return (WIFEXITED(m_Status) != 0) ? WEXITSTATUS(m_Status) : 0; }

  int SystemCall::GetExitSignal() const { return (WIFSIGNALED(m_Status) != 0) ? WTERMSIG(m_Status) : 0; }

  std::string SystemCall::ReadStdout() const { return _ReadOutputStream(m_StdoutFileDescriptor[READ_FD]); }

  std::string SystemCall::ReadStderr() const { return _ReadOutputStream(m_StderrFileDescriptor[READ_FD]); }

  std::string SystemCall::_ReadOutputStream(int fileDescriptor) const
  {
    std::string result;

    if(!m_IsParent || (fileDescriptor == STDIN_FILENO))
    {
      return result;
    }

    std::vector<char> buffer;
    char rawBuffer;
    ssize_t size;
    do
    {
      size = ::read(fileDescriptor, &rawBuffer, sizeof(rawBuffer));
      if(size > 0)
      {
        buffer.push_back(rawBuffer);
      }
    } while((size > 0) && (rawBuffer != '\n'));

    if(size < 0)
    {
      throw Exception::ErrnoError("read()");
    }
    else if(size > 0)
    {
      const auto end = (rawBuffer == '\n') ? (buffer.cend() - 1u) : buffer.cend();
      result         = std::string(buffer.cbegin(), end);
    }

    return result;
  }

  void SystemCall::WriteStdin(const std::string& message) const
  {
    if(!m_IsParent)
    {
      return;
    }

    std::size_t length = message.length();
    if(::write(m_StdinFileDescriptor[WRITE_FD], message.c_str(), length) != static_cast<ssize_t>(length))
    {
      throw Exception::ErrnoError("write()");
    }

    char rawBuffer = '\n';
    length         = sizeof(rawBuffer);
    if(::write(m_StdinFileDescriptor[WRITE_FD], &rawBuffer, length) != static_cast<ssize_t>(length))
    {
      throw Exception::ErrnoError("write()");
    }
  }

  bool SystemCall::IsActive() { return m_IsRunning && !_AwaitExit(false); }

  bool SystemCall::Start()
  {
    if(IsActive())
    {
      return false;
    }

    if((::pipe2(m_StdinFileDescriptor, 0) == -1) || (::pipe2(m_StdoutFileDescriptor, 0) == -1) || (::pipe2(m_StderrFileDescriptor, 0) == -1))
    {
      throw Exception::ErrnoError("pipe2()");
    }

    m_Pid = ::fork();
    if(m_Pid > 0)
    {
      m_IsRunning = true;
      SetupParent();
    }
    else if(m_Pid == 0)
    {
      m_IsParent  = false;
      m_IsRunning = true;
      SetupChild();
    }
    else
    {
      throw Exception::ErrnoError("fork()");
    }

    return true;
  }

  bool SystemCall::Stop(bool force)
  {
    if(!m_IsParent)
    {
      _CloseAllFileDescriptors();
      std::exit(EXIT_SUCCESS);
    }

    if(!IsActive())
    {
      return false;
    }

    if(!SystemCall::SendSignal(m_Pid, force ? SIGTERM : SIGKILL))
    {
      throw Exception::ErrnoError("kill()");
    }

    return _AwaitExit(true);
  }

  void SystemCall::Suspend()
  {
    if(!m_IsParent)
    {
      return;
    }

    if(!SystemCall::SendSignal(m_Pid, SIGSTOP))
    {
      throw Exception::ErrnoError("kill()");
    }
  }

  void SystemCall::Resume()
  {
    if(!m_IsParent)
    {
      return;
    }

    if(!SystemCall::SendSignal(m_Pid, SIGCONT))
    {
      throw Exception::ErrnoError("kill()");
    }
  }

  bool SystemCall::Await(int options)
  {
    if(!m_IsParent)
    {
      return false;
    }

    return _Await(options);
  }

  bool SystemCall::_AwaitExit(bool blocking)
  {
    try
    {
      const bool status = _Await(blocking ? 0 : WNOHANG);
      if(status)
      {
        m_IsRunning = false;
        _CloseAllFileDescriptors();
      }

      return status;
    }
    catch(const Exception::ErrnoError& e)
    {
      if(e.GetCode() != ECHILD)
      {
        throw;
      }

      m_IsRunning = false;
      _CloseAllFileDescriptors();
      return true;
    }
  }

  bool SystemCall::_Await(int options)
  {
    if(!m_IsParent)
    {
      throw;
    }

    pid_t pid = ::waitpid(m_Pid, &m_Status, options);
    if(pid == -1)
    {
      throw Exception::ErrnoError("waitpid()");
    }

    return pid == m_Pid;
  }

  bool SystemCall::Signal(int signal)
  {
    if(!m_IsParent)
    {
      return false;
    }

    return SystemCall::SendSignal(m_Pid, signal);
  }

  void SystemCall::SetupParent()
  {
    if(!m_IsParent)
    {
      return;
    }

    SystemCall::_CloseFileDescriptor(m_StdinFileDescriptor[READ_FD]);
    SystemCall::_CloseFileDescriptor(m_StdoutFileDescriptor[WRITE_FD]);
    SystemCall::_CloseFileDescriptor(m_StderrFileDescriptor[WRITE_FD]);
  }

  void SystemCall::SetupChild()
  {
    if(m_IsParent)
    {
      return;
    }

    m_Pid = ::getppid(); // Parent process ID
    std::fflush(nullptr);

    // Stdin
    if(::dup3(m_StdinFileDescriptor[READ_FD], STDIN_FILENO, 0) == -1)
    {
      throw Exception::ErrnoError("dup3()");
    }
    SystemCall::_CloseFileDescriptorPair(m_StdinFileDescriptor);

    // Stdout
    if(::dup3(m_StdoutFileDescriptor[WRITE_FD], STDOUT_FILENO, 0) == -1)
    {
      throw Exception::ErrnoError("dup3()");
    }
    SystemCall::_CloseFileDescriptorPair(m_StdoutFileDescriptor);

    // Stderr
    if(::dup3(m_StderrFileDescriptor[WRITE_FD], STDERR_FILENO, 0) == -1)
    {
      throw Exception::ErrnoError("dup3()");
    }
    SystemCall::_CloseFileDescriptorPair(m_StderrFileDescriptor);

    std::vector<char*> argumentList;
    argumentList.reserve(m_ArgumentList.size() + 1u);
    SystemCall::_GenerateRawArgumentList(m_ArgumentList, argumentList);
    std::vector<char*> environmentList;
    environmentList.reserve(m_EnvironmentList.size() + 1u);
    SystemCall::_GenerateRawArgumentList(m_EnvironmentList, environmentList);
    if(::execvpe(argumentList[0], &argumentList[0], &environmentList[0]) == -1)
    {
      throw Exception::ErrnoError("execvpe()");
    }
  }

  void SystemCall::_CloseAllFileDescriptors()
  {
    SystemCall::_CloseFileDescriptorPair(m_StdinFileDescriptor);
    SystemCall::_CloseFileDescriptorPair(m_StdoutFileDescriptor);
    SystemCall::_CloseFileDescriptorPair(m_StderrFileDescriptor);
  }

  void SystemCall::_CloseFileDescriptor(int& fileDescriptor)
  {
    if(fileDescriptor != -1)
    {
      if(::close(fileDescriptor) == -1)
      {
        throw Exception::ErrnoError("close()");
      }

      fileDescriptor = -1;
    }
  }

  void SystemCall::_CloseFileDescriptorPair(int (&fileDescriptors)[2])
  {
    SystemCall::_CloseFileDescriptor(fileDescriptors[READ_FD]);
    SystemCall::_CloseFileDescriptor(fileDescriptors[WRITE_FD]);
  }

  void SystemCall::_GenerateRawArgumentList(const std::vector<std::string>& source, std::vector<std::unique_ptr<char[]>>& result)
  {
    std::transform(source.cbegin(), source.cend(), std::back_inserter(result), [](const std::string& element) {
      std::unique_ptr<char[]> ptr = std::make_unique<char[]>(element.length() + 1u);
      std::size_t length          = element.copy(ptr.get(), element.length());
      ptr.get()[length]           = '\0';
      return std::move(ptr);
    });
  }

  void SystemCall::_GenerateRawArgumentList(const std::vector<std::unique_ptr<char[]>>& source, std::vector<char*>& result)
  {
    std::transform(source.cbegin(), source.cend(), std::back_inserter(result), [](const std::unique_ptr<char[]>& element) { return element.get(); });
    result.push_back(nullptr);
  }

  SystemCall::SystemCall(const std::string& executableFilepath, const std::vector<std::string>& argumentList, const std::vector<std::string>& environmentList)
      : m_ExecutableFilepath(executableFilepath)
      , m_ArgumentList()
      , m_EnvironmentList()
      , m_Pid(-1)
      , m_Status(0)
      , m_StdinFileDescriptor {-1, -1}
      , m_StdoutFileDescriptor {-1, -1}
      , m_StderrFileDescriptor {-1, -1}
      , m_IsParent(true)
      , m_IsRunning(false)
  {
    m_ArgumentList.reserve(m_ArgumentList.size() + 1u);
    m_EnvironmentList.reserve(m_EnvironmentList.size());

    std::unique_ptr<char[]> ptr = std::make_unique<char[]>(m_ExecutableFilepath.length() + 1u);
    std::size_t length          = m_ExecutableFilepath.copy(ptr.get(), m_ExecutableFilepath.length());
    ptr.get()[length]           = '\0';
    m_ArgumentList.push_back(std::move(ptr));

    SystemCall::_GenerateRawArgumentList(argumentList, m_ArgumentList);
    SystemCall::_GenerateRawArgumentList(environmentList, m_EnvironmentList);
  }

  SystemCall::~SystemCall() { Stop(); }
} // namespace Util
