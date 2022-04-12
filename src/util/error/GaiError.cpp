#include "GaiError.hpp"

#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>

namespace Util::Error
{
  std::string GaiError::ToString() const { return gai_strerror(GetValue()); }

  GaiError::GaiError(int value)
      : ErrorBase<int>(value)
  {}
} // namespace Util::Error
