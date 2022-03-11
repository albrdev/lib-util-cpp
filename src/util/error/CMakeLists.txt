cmake_minimum_required(VERSION 3.14)

target_sources(${LIBRARY_UTIL}
  PUBLIC
  IError.hpp
  ErrorBase.hpp
  StringError.hpp
  ErrnoError.hpp
  GaiError.hpp
  Error.hpp

  PRIVATE
  StringError.cpp
  ErrnoError.cpp
  GaiError.cpp
)

target_sources(${UNITTEST_UTIL}
  PRIVATE
  Error.test.cpp
)
