cmake_minimum_required(VERSION 3.14)

target_sources(${LIBRARY_UTIL}
  PUBLIC
  IRdsObject.hpp
  RdsObject.hpp
  Rds.hpp

  PRIVATE
  Rds.cpp
)

target_sources(${UNITTEST_UTIL}
  PRIVATE
  Rds.test.cpp
)
