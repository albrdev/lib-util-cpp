#ifndef __UTIL_ERROR__STRINGERROR_HPP__
#define __UTIL_ERROR__STRINGERROR_HPP__

#include "ErrorBase.hpp"

#include <string>

namespace Util::Error
{
  class StringError : public ErrorBase<std::string>
  {
    public:
    virtual std::string ToString() const override;

    StringError();
    StringError(const std::string& value);

    virtual ~StringError() override = default;
  };
} // namespace Util::Error

#endif // __UTIL_ERROR__STRINGERROR_HPP__
