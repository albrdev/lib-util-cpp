cmake_minimum_required(VERSION 3.14)

target_sources(${LIBRARY_UTIL}
  PUBLIC
  Common.hpp

  PRIVATE
)

target_sources(${UNITTEST_UTIL}
  PRIVATE
  Common.test.cpp
)

add_subdirectory(rds)
