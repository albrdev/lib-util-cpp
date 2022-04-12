#ifndef __UTIL_ERROR__ERROR_HPP__
#define __UTIL_ERROR__ERROR_HPP__

#include "ErrnoError.hpp"
#include "ErrorBase.hpp"
#include "GaiError.hpp"
#include "IError.hpp"
#include "StringError.hpp"

#include <cstddef>
#include <memory>

namespace Util::Error
{
  using BoolError = ErrorBase<bool>;
  using IntError  = ErrorBase<int>;

  namespace
  {
    using DefaultError = ErrorBase<std::nullptr_t>;

    static const thread_local std::shared_ptr<DefaultError> __default = std::make_shared<DefaultError>();
    static thread_local std::shared_ptr<IError> __instance            = __default;
  } // namespace

  inline const IError& Get() { return *__instance; }

  template<class T, class... TArgs>
  inline void Set(TArgs... args)
  {
    __instance = std::make_shared<T>(args...);
  }

  inline void Clear() { __instance = __default; }
} // namespace Util::Error

#endif // __UTIL_ERROR__ERROR_HPP__
