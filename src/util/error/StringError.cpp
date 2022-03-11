#include "StringError.hpp"

namespace Util::Error
{
  std::string StringError::ToString() const { return GetValue(); }

  StringError::StringError()
      : ErrorBase<std::string>()
  {}

  StringError::StringError(const std::string& value)
      : ErrorBase<std::string>(value)
  {}
} // namespace Util::Error
