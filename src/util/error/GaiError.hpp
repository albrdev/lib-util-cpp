#ifndef __GAIERROR_HPP__
#define __GAIERROR_HPP__

#include "ErrorBase.hpp"

namespace Error
{
  class GaiError : public ErrorBase<int>
  {
    public:
    virtual std::string ToString() const override;

    GaiError(int value);

    virtual ~GaiError() override = default;
  };
} // namespace Error

#endif // __GAIERROR_HPP__
