#ifndef __UTIL_EXCEPTION__ERRNOERROR_HPP__
#define __UTIL_EXCEPTION__ERRNOERROR_HPP__

#include "SystemError.hpp"

#include <cerrno>
#include <type_traits>

namespace Util::Exception
{
  using error_t = std::remove_reference<decltype(errno)>::type;

  class ErrnoError : public SystemError<error_t>
  {
    public:
    ErrnoError(const std::string& prefix = "", error_t errorCode = errno);
    ErrnoError(const ErrnoError& other) noexcept;
    virtual ~ErrnoError() override = default;

    private:
    static std::string CreateMessage(const std::string& prefix, error_t errorCode);
  };
} // namespace Util::Exception

#endif // __UTIL_EXCEPTION__ERRNOERROR_HPP__
