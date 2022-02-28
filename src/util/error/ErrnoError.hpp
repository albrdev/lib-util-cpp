#ifndef __ERRNOERROR_HPP__
#define __ERRNOERROR_HPP__

#include <cstring>
#include <cerrno>
#include <type_traits>
#include "ErrorBase.hpp"

namespace Error
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
} // namespace Error

#endif // __ERRNOERROR_HPP__
