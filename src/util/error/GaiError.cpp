#include "GaiError.hpp"
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

namespace Error
{
  std::string GaiError::ToString() const { return gai_strerror(GetValue()); }

  GaiError::GaiError(int value)
      : ErrorBase<int>(value)
  {}
} // namespace Error
