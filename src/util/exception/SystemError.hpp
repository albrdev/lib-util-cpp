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
    T GetCode() const { return m_Code; }

    SystemError(const std::string& message, T code)
        : std::runtime_error(message)
        , m_Code(code)
    {}

    SystemError(const SystemError& other) noexcept
        : std::runtime_error(other)
        , m_Code(other.m_Code)
    {}

    virtual ~SystemError() override = default;

    static const T Default;

    private:
    const T m_Code;
  };

  template<class T>
  const T SystemError<T>::Default = T();
} // namespace Util::Exception

#endif // __UTIL_EXCEPTION__SYSTEMERROR_HPP__
