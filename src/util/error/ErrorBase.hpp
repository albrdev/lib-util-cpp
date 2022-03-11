#ifndef __UTIL_ERROR__ERRORBASE_HPP__
#define __UTIL_ERROR__ERRORBASE_HPP__

#include "IError.hpp"

namespace Util::Error
{
  template<class T>
  class ErrorBase : public IError
  {
    public:
    virtual operator bool() const override { return m_Value != m_Default; }

    virtual const T& GetValue() const { return m_Value; }
    virtual void SetValue(const T& value) { m_Value = value; }
    virtual void Clear() override { m_Value = T(); }

    virtual std::string ToString() const override { return std::string(); }

    ErrorBase()
        : IError()
        , m_Value()
    {}

    ErrorBase(const T& value)
        : IError()
        , m_Value(value)
    {}

    virtual ~ErrorBase() override = default;

    private:
    T m_Value;
    static const T m_Default;
  };

  template<class T>
  const T ErrorBase<T>::m_Default = T();
} // namespace Util::Error

#endif // __UTIL_ERROR__ERRORBASE_HPP__
