#ifndef __ERRORBASE_HPP__
#define __ERRORBASE_HPP__

#include <memory>
#include <mutex>
#include "IError.hpp"

namespace Error
{
  template<class T>
  class ErrorBase : public IError
  {
    public:
    virtual operator bool() const override { return m_Value != T(); }

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
  };
} // namespace Error

#endif // __ERRORBASE_HPP__
