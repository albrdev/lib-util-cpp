cmake_minimum_required(VERSION 3.14)

target_sources(${LIBRARY_UTIL}
  PUBLIC
  Version.hpp

  PRIVATE
  Version.cpp
)

target_sources(${UNITTEST_UTIL}
  PRIVATE
  Version.test.cpp
)
