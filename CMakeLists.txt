cmake_minimum_required(VERSION 3.14)

#set(CMAKE_C_STANDARD 98)
#set(CMAKE_C_STANDARD_REQUIRED True)
#set(CMAKE_C_COMPILER "gcc")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
#set(CMAKE_CXX_COMPILER "g++")

set(LIBRARY_UTIL util)
set(UNITTEST_UTIL util-test)

if(TARGET ${LIBRARY_UTIL})
    return()
endif()

project(lib-util VERSION 1.0.0)

if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wconversion -Wpedantic -Wno-psabi")
elseif(MSVC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
endif()

add_library(${LIBRARY_UTIL} STATIC)
set_target_properties(${LIBRARY_UTIL} PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(${LIBRARY_UTIL} PUBLIC src)

add_library(${UNITTEST_UTIL} STATIC)
target_include_directories(${UNITTEST_UTIL} PUBLIC src)
target_link_libraries(${UNITTEST_UTIL} gtest_main gmock_main)

# Traverse directories
add_subdirectory(src)

set(PROJECT_TEST unit_testsuite-util)
set(EXECUTABLE_TEST unit_testsuite-util)

project(${PROJECT_TEST})

include(FetchContent)
FetchContent_Declare(
  googletest
  # Specify the commit you depend on and update it regularly.
  URL https://github.com/google/googletest/archive/refs/tags/release-1.11.0.zip
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

get_target_property(SOURCES ${UNITTEST_UTIL} SOURCES)
add_executable(${EXECUTABLE_TEST} main-test.cpp ${SOURCES})
target_link_libraries(${EXECUTABLE_TEST} ${LIBRARY_UTIL} ${UNITTEST_UTIL})
enable_testing()
add_test(NAME ${PROJECT_TEST} COMMAND ${EXECUTABLE_TEST})
