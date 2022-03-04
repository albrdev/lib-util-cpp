#include "Version.hpp"

namespace Util
{
  bool Version::operator==(const Version& rhs) { return (m_Major == rhs.m_Major) && (m_Minor == rhs.m_Minor) && (m_Build == rhs.m_Build); }
  bool Version::operator!=(const Version& rhs) { return (m_Major != rhs.m_Major) || (m_Minor != rhs.m_Minor) || (m_Build != rhs.m_Build); }

  bool Version::operator>(const Version& rhs) { return (m_Major > rhs.m_Major) || (m_Minor > rhs.m_Minor) || (m_Build > rhs.m_Build); }
  bool Version::operator<(const Version& rhs) { return (m_Major < rhs.m_Major) || (m_Minor < rhs.m_Minor) || (m_Build < rhs.m_Build); }

  bool Version::operator>=(const Version& rhs) { return (m_Major >= rhs.m_Major) || (m_Minor >= rhs.m_Minor) || (m_Build >= rhs.m_Build); }
  bool Version::operator<=(const Version& rhs) { return (m_Major <= rhs.m_Major) || (m_Minor <= rhs.m_Minor) || (m_Build <= rhs.m_Build); }

  Version Version::operator+(const Version& rhs) const { return Version(m_Major + rhs.m_Major, m_Minor + rhs.m_Minor, m_Build + rhs.m_Build); }
  Version Version::operator-(const Version& rhs) const { return Version(m_Major - rhs.m_Major, m_Minor - rhs.m_Minor, m_Build - rhs.m_Build); }

  Version Version::operator<<(const Version& rhs) const
  {
    if(rhs.m_Major != 0u)
    {
      return Version(m_Major + rhs.m_Major, 0u, 0u);
    }
    else if(rhs.m_Minor != 0u)
    {
      return Version(m_Major, m_Minor + rhs.m_Minor, 0u);
    }
    else if(rhs.m_Build != 0)
    {
      return Version(m_Major, m_Minor, m_Build + rhs.m_Build);
    }

    return *this;
  }

  Version Version::operator>>(const Version& rhs) const
  {
    if(rhs.m_Major != 0u)
    {
      return Version(m_Major - rhs.m_Major, 0u, 0u);
    }
    else if(rhs.m_Minor != 0u)
    {
      return Version(m_Major, m_Minor - rhs.m_Minor, 0u);
    }
    else if(rhs.m_Build != 0)
    {
      return Version(m_Major, m_Minor, m_Build - rhs.m_Build);
    }

    return *this;
  }

  Version& Version::operator+=(const Version& rhs)
  {
    m_Major += rhs.m_Major;
    m_Minor += rhs.m_Minor;
    m_Minor += rhs.m_Minor;

    return *this;
  }

  Version& Version::operator-=(const Version& rhs)
  {
    m_Major -= rhs.m_Major;
    m_Minor -= rhs.m_Minor;
    m_Build -= rhs.m_Build;

    return *this;
  }

  std::uint32_t Version::GetMajor() const { return m_Major; }
  std::uint32_t Version::GetMinor() const { return m_Minor; }
  std::uint32_t Version::GetBuild() const { return m_Build; }

  Version::Version(std::uint32_t major, std::uint32_t minor, std::uint32_t build)
      : m_Major(major)
      , m_Minor(minor)
      , m_Build(build)
  {}

  Version::Version()
      : m_Major(0u)
      , m_Minor(0u)
      , m_Build(0u)
  {}

  Version::Version(const Version& other)
      : m_Major(other.m_Major)
      , m_Minor(other.m_Minor)
      , m_Build(other.m_Build)
  {}

  Version::Version(Version&& other)
      : m_Major(std::move(other.m_Major))
      , m_Minor(std::move(other.m_Minor))
      , m_Build(std::move(other.m_Build))
  {}

  Version& Version::operator=(const Version& other)
  {
    m_Major = other.m_Major;
    m_Minor = other.m_Minor;
    m_Build = other.m_Build;

    return *this;
  }

  Version& Version::operator=(Version&& other)
  {
    m_Major = std::move(other.m_Major);
    m_Minor = std::move(other.m_Minor);
    m_Build = std::move(other.m_Build);

    return *this;
  }
} // namespace Util
