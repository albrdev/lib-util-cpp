#include "ErrnoError.hpp"

#include <cstring>

namespace Util::Exception
{
  ErrnoError::ErrnoError(const std::string& prefix, error_t code)
      : SystemError(ErrnoError::CreateMessage(prefix, code), code)
  {}

  ErrnoError::ErrnoError(const ErrnoError& other) noexcept
      : SystemError(other)
  {}

  std::string ErrnoError::CreateMessage(const std::string& prefix, error_t code) { return (prefix.empty() ? "" : (prefix + ": ")) + std::strerror(code); }
} // namespace Util::Exception
