#ifndef __UTIL_VERSION__VERSION_HPP__
#define __UTIL_VERSION__VERSION_HPP__

#include <cstdint>
#include <istream>
#include <ostream>
#include <sstream>
#include <string>

namespace Util::Version
{
  class Version
  {
    public:
    friend std::istream& operator>>(std::istream& stream, Version& version)
    {
      stream >> version.m_Major;
      stream.ignore(1, '.');
      stream >> version.m_Minor;
      stream.ignore(1, '.');
      stream >> version.m_Build;
      return stream;
    }

    friend std::ostream& operator<<(std::ostream& stream, const Version& version)
    {
      return (stream << version.m_Major << '.' << version.m_Minor << '.' << version.m_Build);
    }

    bool operator==(const Version& rhs);
    bool operator!=(const Version& rhs);

    bool operator>(const Version& rhs);
    bool operator<(const Version& rhs);

    bool operator>=(const Version& rhs);
    bool operator<=(const Version& rhs);

    Version operator+(const Version& rhs) const;
    Version operator-(const Version& rhs) const;

    Version operator<<(const Version& rhs) const;
    Version operator>>(const Version& rhs) const;

    Version& operator+=(const Version& rhs);
    Version& operator-=(const Version& rhs);

    std::uint32_t GetMajor() const;
    std::uint32_t GetMinor() const;
    std::uint32_t GetBuild() const;

    Version(std::uint32_t major, std::uint32_t minor, std::uint32_t build);
    virtual ~Version() = default;

    Version();
    Version(const Version& other);
    Version(Version&& other);
    Version& operator=(const Version& other);
    Version& operator=(Version&& other);

    private:
    std::uint32_t m_Major;
    std::uint32_t m_Minor;
    std::uint32_t m_Build;
  };
} // namespace Util::Version

#endif // __UTIL_VERSION__VERSION_HPP__
