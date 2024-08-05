#include "SystemCall.hpp"

#include <algorithm>
#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <iterator>

#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define READ_INDEX  0
#define WRITE_INDEX 1

namespace Util
{
  int SystemCall::GetStatus() const { return (WIFEXITED(m_Status) != 0) ? WEXITSTATUS(m_Status) : 0; }

  int SystemCall::GetSignal() const { return (WIFSIGNALED(m_Status) != 0) ? WTERMSIG(m_Status) : 0; }

  bool SystemCall::ReadStdout(std::string& result) const { return _ReadStream(result, m_StdoutFileDescriptor[READ_INDEX]); }

  bool SystemCall::ReadStderr(std::string& result) const { return _ReadStream(result, m_StderrFileDescriptor[READ_INDEX]); }

  bool SystemCall::_ReadStream(std::string& result, int fileDescriptor) const
  {
    if(!m_IsParent || (fileDescriptor == STDIN_FILENO))
    {
      return false;
    }

    std::vector<char> buffer;
    char rawBuffer;
    int nbytes;
    do
    {
      nbytes = read(fileDescriptor, &rawBuffer, sizeof(rawBuffer));
      buffer.push_back(rawBuffer);
    } while(nbytes > 0 && rawBuffer != '\n');

    if(nbytes < 0)
    {
      return false;
    }
    else if(nbytes >= 0)
    {
      auto end = (rawBuffer == '\n') ? (buffer.end() - 1u) : buffer.end();
      result   = std::string(buffer.begin(), end);
      return true;
    }
    else
    {
      return true;
    }
  }

  bool SystemCall::WriteStdin(const std::string& message) const
  {
    if(!m_IsParent)
    {
      return false;
    }

    std::size_t length = message.length();
    if(write(m_StdinFileDescriptor[WRITE_INDEX], message.c_str(), length) != static_cast<ssize_t>(length))
    {
      return false;
    }

    char rawBuffer = '\n';
    length         = sizeof(rawBuffer);
    if(write(m_StdinFileDescriptor[WRITE_INDEX], &rawBuffer, length) != static_cast<ssize_t>(length))
    {
      return false;
    }

    return true;
  }

  bool SystemCall::IsActive() { return (m_Pid != -1) && !_Await(WNOHANG); }

  bool SystemCall::Start()
  {
    if(m_Pid != -1)
    {
      return false;
    }

    if((pipe2(m_StdinFileDescriptor, 0) == -1) || (pipe2(m_StdoutFileDescriptor, 0) == -1) || (pipe2(m_StderrFileDescriptor, 0) == -1))
    {
      // Error
      std::cout << "*** Error: pipe2(): " << strerror(errno) << std::endl;
      return false;
    }

    m_Pid = fork();
    //m_Pid = 0;
    if(m_Pid > 0)
    {
      SetupParent();
    }
    else if(m_Pid == 0)
    {
      m_IsParent = false;
      SetupChild();
    }
    else
    {
      // Error
      std::cout << "*** Error: fork(): " << strerror(errno) << std::endl;
      return false;
    }

    return true;
  }

  bool SystemCall::Stop(int signal)
  {
    if(!m_IsParent)
    {
      if(!_CloseAllFileDescriptors())
      {
        std::cout << "*** Error: close(): " << strerror(errno) << std::endl;
        return false;
      }

      std::exit(EXIT_SUCCESS);
    }

    if(signal == 0)
    {
      return false;
    }

    if(!IsActive())
    {
      return false;
    }

    if(kill(m_Pid, signal) == -1)
    {
      return false;
    }

    return Await();
  }

  bool SystemCall::Await()
  {
    if(!m_IsParent)
    {
      return false;
    }

    if(!_Await())
    {
      return false;
    }

    return _CloseAllFileDescriptors();
  }

  bool SystemCall::_Await(int options)
  {
    if(!m_IsParent)
    {
      return false;
    }

    pid_t pid = waitpid(m_Pid, &m_Status, options);
    if(pid == -1)
    {
      if(errno == ECHILD)
      {
        m_Pid = -1;
        return true;
      }
      else
      {
        // Error
        std::cout << "*** Error: waitpid(): " << strerror(errno) << std::endl;
        return false;
      }
    }

    if(pid == m_Pid)
    {
      m_Pid = -1;
      return true;
    }
    else
    {
      return false;
    }
  }

  bool SystemCall::SetupParent()
  {
    if(!m_IsParent)
    {
      return false;
    }

    if(!SystemCall::_CloseFileDescriptor(m_StdinFileDescriptor[READ_INDEX]))
    {
      std::cout << "*** Error: close(): " << strerror(errno) << std::endl;
      return false;
    }

    if(!SystemCall::_CloseFileDescriptor(m_StdoutFileDescriptor[WRITE_INDEX]))
    {
      std::cout << "*** Error: close(): " << strerror(errno) << std::endl;
      return false;
    }

    if(!SystemCall::_CloseFileDescriptor(m_StderrFileDescriptor[WRITE_INDEX]))
    {
      std::cout << "*** Error: close(): " << strerror(errno) << std::endl;
      return false;
    }

    return true;
  }

  bool SystemCall::SetupChild()
  {
    if(m_IsParent)
    {
      return false;
    }

    m_Pid = getppid(); // Parent process ID
    std::fflush(nullptr);

    // Stdin
    if(dup3(m_StdinFileDescriptor[READ_INDEX], STDIN_FILENO, 0) == -1)
    {
      // Error
      std::cout << "*** Error: dup3(): " << strerror(errno) << std::endl;
      return false;
    }

    if(!SystemCall::_CloseFileDescriptorPair(m_StdinFileDescriptor))
    {
      std::cout << "*** Error: close(): " << strerror(errno) << std::endl;
      return false;
    }

    // Stdout
    if(dup3(m_StdoutFileDescriptor[WRITE_INDEX], STDOUT_FILENO, 0) == -1)
    {
      // Error
      std::cout << "*** Error: dup3(): " << strerror(errno) << std::endl;
      return false;
    }

    if(!SystemCall::_CloseFileDescriptorPair(m_StdoutFileDescriptor))
    {
      std::cout << "*** Error: close(): " << strerror(errno) << std::endl;
      return false;
    }

    // Stderr
    if(dup3(m_StderrFileDescriptor[WRITE_INDEX], STDERR_FILENO, 0) == -1)
    {
      // Error
      std::cout << "*** Error: dup3(): " << strerror(errno) << std::endl;
      return false;
    }

    if(!SystemCall::_CloseFileDescriptorPair(m_StderrFileDescriptor))
    {
      std::cout << "*** Error: close(): " << strerror(errno) << std::endl;
      return false;
    }

    std::vector<char*> argumentList;
    argumentList.reserve(m_ArgumentList.size() + 2u);
    std::transform(m_ArgumentList.cbegin(), m_ArgumentList.cend(), std::back_inserter(argumentList), [](const std::unique_ptr<char[]>& element) {
      return element.get();
    });
    argumentList.push_back(nullptr);

    std::vector<char*> environmentList;
    environmentList.reserve(m_EnvironmentList.size() + 2u);
    std::transform(m_EnvironmentList.cbegin(), m_EnvironmentList.cend(), std::back_inserter(environmentList), [](const std::unique_ptr<char[]>& element) {
      return element.get();
    });
    environmentList.push_back(nullptr);

    if(execvpe(argumentList[0], &argumentList[0], &environmentList[0]) == -1)
    {
      // Error
      std::cout << "*** Error: execvpe(): " << strerror(errno) << ": " << m_ExecutableFilepath << std::endl;
      return false;
    }

    return true;
  }

  bool SystemCall::_CloseAllFileDescriptors()
  {
    if(!SystemCall::_CloseFileDescriptorPair(m_StdinFileDescriptor))
    {
      return false;
    }

    if(!SystemCall::_CloseFileDescriptorPair(m_StdoutFileDescriptor))
    {
      return false;
    }

    if(!SystemCall::_CloseFileDescriptorPair(m_StderrFileDescriptor))
    {
      return false;
    }

    return true;
  }

  bool SystemCall::_CloseFileDescriptor(int& fileDescriptor)
  {
    if(fileDescriptor != -1)
    {
      if(close(fileDescriptor) == -1)
      {
        return false;
      }

      fileDescriptor = -1;
    }

    return true;
  }

  bool SystemCall::_CloseFileDescriptorPair(int (&fileDescriptors)[2])
  {
    return SystemCall::_CloseFileDescriptor(fileDescriptors[0]) && SystemCall::_CloseFileDescriptor(fileDescriptors[1]);
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
  {
    std::unique_ptr<char[]> ptr = std::make_unique<char[]>(m_ExecutableFilepath.length() + 1u);
    std::size_t length          = m_ExecutableFilepath.copy(ptr.get(), m_ExecutableFilepath.length());
    ptr.get()[length]           = '\0';
    m_ArgumentList.push_back(std::move(ptr));

    std::transform(argumentList.cbegin(), argumentList.cend(), std::back_inserter(m_ArgumentList), [](const std::string& element) {
      std::unique_ptr<char[]> ptr = std::make_unique<char[]>(element.length() + 1u);
      std::size_t length          = element.copy(ptr.get(), element.length());
      ptr.get()[length]           = '\0';
      return std::move(ptr);
    });

    std::transform(environmentList.cbegin(), environmentList.cend(), std::back_inserter(m_EnvironmentList), [](const std::string& element) {
      std::unique_ptr<char[]> ptr = std::make_unique<char[]>(element.length() + 1u);
      std::size_t length          = element.copy(ptr.get(), element.length());
      ptr.get()[length]           = '\0';
      return std::move(ptr);
    });
  }

  SystemCall::~SystemCall() { Stop(); }
} // namespace Util
