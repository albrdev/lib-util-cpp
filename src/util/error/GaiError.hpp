#ifndef __UTIL_ERROR__GAIERROR_HPP__
#define __UTIL_ERROR__GAIERROR_HPP__

#include "ErrorBase.hpp"

namespace Util::Error
{
  class GaiError : public ErrorBase<int>
  {
    public:
    virtual std::string ToString() const override;

    GaiError(int value);

    virtual ~GaiError() override = default;
  };
} // namespace Util::Error

#endif // __UTIL_ERROR__GAIERROR_HPP__
