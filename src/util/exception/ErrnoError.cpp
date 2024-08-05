#include "ErrnoError.hpp"

#include <cstring>

namespace Util::Exception
{
  ErrnoError::ErrnoError(error_t errorCode)
      : SystemError(ErrnoError::CreateMessage(errorCode), errorCode)
  {}

  ErrnoError::ErrnoError(const ErrnoError& other) noexcept
      : SystemError(other)
  {}

  std::string ErrnoError::CreateMessage(error_t errorCode) { return std::strerror(errorCode); }
} // namespace Util::Exception
