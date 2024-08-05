cmake_minimum_required(VERSION 3.14)

target_sources(${LIBRARY_UTIL}
  PUBLIC
  SystemError.hpp
  ErrnoError.hpp

  PRIVATE
  ErrnoError.cpp
)

target_sources(${UNITTEST_UTIL}
  PRIVATE
  SystemError.test.cpp
  ErrnoError.test.cpp
)
