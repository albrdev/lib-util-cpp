#ifndef __ERROR_HPP__
#define __ERROR_HPP__

#include <unordered_map>
#include <memory>
#include <thread>
#include <mutex>
#include "IError.hpp"
#include "ErrorBase.hpp"
#include "ErrnoError.hpp"
#include "GaiError.hpp"

namespace Error
{
  using BoolError   = ErrorBase<bool>;
  using IntError    = ErrorBase<int>;
  using StringError = ErrorBase<std::string>;

  static std::unordered_map<std::thread::id, std::unique_ptr<IError>> __errorMap;
  static std::mutex __errorMutex;

  template<class T, class... U>
  inline std::unique_ptr<IError> Make(U... args)
  {
    return std::make_unique<T>(args...);
  }

  inline const IError* Get()
  {
    std::lock_guard<std::mutex> lock(__errorMutex);
    const auto& iter = __errorMap.find(std::this_thread::get_id());
    return iter != __errorMap.cend() ? iter->second.get() : nullptr;
  }

  template<class T, class... U>
  inline void Set(U... args)
  {
    std::lock_guard<std::mutex> lock(__errorMutex);
    __errorMap[std::this_thread::get_id()] = Make<T>(args...);
  }

  inline void Clear()
  {
    std::lock_guard<std::mutex> lock(__errorMutex);
    const auto& iter = __errorMap.find(std::this_thread::get_id());
    if(iter != __errorMap.cend())
    {
      iter->second.reset();
    }
  }
} // namespace Error

#endif // __ERROR_HPP__
