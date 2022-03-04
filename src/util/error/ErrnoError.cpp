#include "ErrnoError.hpp"
#include <cstring>

namespace Util::Error
{
  std::string ErrnoError::ToString() const { return std::strerror(GetValue()); }

  ErrnoError::ErrnoError()
      : ErrorBase<errno_t>(errno)
  {}

  ErrnoError::ErrnoError(int value)
      : ErrorBase<errno_t>(value)
  {}
} // namespace Util::Error
