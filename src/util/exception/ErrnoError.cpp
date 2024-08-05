#include "ErrnoError.hpp"

#include <cstring>

namespace Util::Exception
{
  ErrnoError::ErrnoError(const std::string& prefix, error_t errorCode)
      : SystemError(ErrnoError::CreateMessage(prefix, errorCode), errorCode)
  {}

  ErrnoError::ErrnoError(const ErrnoError& other) noexcept
      : SystemError(other)
  {}

  std::string ErrnoError::CreateMessage(const std::string& prefix, error_t errorCode)
  {
    return (prefix.empty() ? "" : (prefix + ": ")) + std::strerror(errorCode);
  }
} // namespace Util::Exception
