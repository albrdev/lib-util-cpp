cmake_minimum_required(VERSION 3.14)

target_sources(${LIBRARY_UTIL}
  PUBLIC
  SystemCall.hpp

  PRIVATE
  SystemCall.cpp
)

target_sources(${UNITTEST_UTIL}
  PRIVATE
  SystemCall.test.cpp
)

add_subdirectory(error)
add_subdirectory(random)
add_subdirectory(version)
add_subdirectory(exception)
