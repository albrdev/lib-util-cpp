#ifndef __UTIL_ERROR__ERRNOERROR_HPP__
#define __UTIL_ERROR__ERRNOERROR_HPP__

#include "ErrorBase.hpp"

#include <cerrno>
#include <string>
#include <type_traits>

namespace Util::Error
{
  using errno_t = std::remove_reference<decltype(errno)>::type;

  class ErrnoError : public ErrorBase<errno_t>
  {
    public:
    virtual std::string ToString() const override;

    ErrnoError();
    ErrnoError(int value);

    virtual ~ErrnoError() override = default;
  };
} // namespace Util::Error

#endif // __UTIL_ERROR__ERRNOERROR_HPP__
