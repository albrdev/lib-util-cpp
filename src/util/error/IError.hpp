#ifndef __IERROR_HPP__
#define __IERROR_HPP__

#include <string>
#include <ostream>

namespace Error
{
  class IError
  {
    public:
    friend std::ostream& operator<<(std::ostream& stream, const IError& self);

    virtual operator bool() const = 0;

    virtual void Clear()                 = 0;
    virtual std::string ToString() const = 0;

    virtual ~IError() = default;

    protected:
    IError() = default;

    private:
    IError(const IError&) = delete;
    IError& operator=(const IError&) = delete;
  };

  inline std::ostream& operator<<(std::ostream& stream, const IError& self) { return stream << self.ToString(); }
} // namespace Error

#endif // __IERROR_HPP__