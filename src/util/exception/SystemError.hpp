#ifndef __UTIL_EXCEPTION__SYSTEMERROR_HPP__
#define __UTIL_EXCEPTION__SYSTEMERROR_HPP__

#include <stdexcept>
#include <string>

namespace Util::Exception
{
  template<class T>
  class SystemError : public std::runtime_error
  {
    public:
    T GetErrorCode() const { return m_ErrorCode; }

    SystemError(const std::string& message, T errorCode)
        : std::runtime_error(message)
        , m_ErrorCode(errorCode)
    {}

    SystemError(const SystemError& other) noexcept
        : std::runtime_error(other)
        , m_ErrorCode(other.m_ErrorCode)
    {}

    virtual ~SystemError() override = default;

    static const T Default;

    private:
    const T m_ErrorCode;
  };

  template<class T>
  const T SystemError<T>::Default = T();
} // namespace Util::Exception

#endif // __UTIL_EXCEPTION__SYSTEMERROR_HPP__
