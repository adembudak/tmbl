cmake_minimum_required(VERSION 3.17)

project(
  tombul
  DESCRIPTION "A GameBoy emulator implementation"
  HOMEPAGE_URL https://github.com/adembudak/tombul
  LANGUAGES CXX)

option(ENABLE_INCLUDE_WHAT_YOU_USE "Run iwyu on source" OFF)
option(ENABLE_CLANG_TIDY "Run clang-tidy on source" OFF)
option(ENABLE_CPPCHECK "Run cppcheck on source" OFF)
option(ENABLE_CCACHE "Cache build with ccache" OFF)

option(ENABLE_ROM_TESTS "Run test ROMs" OFF)
option(ENABLE_UNIT_TESTS "Build unit tests" OFF)

include(CTest)
include(ExternalProject)

set(CMAKE_CXX_STANDARD 17 CACHE STRING "")
set(CMAKE_CXX_STANDARD_REQUIRED YES CACHE BOOL "")
set(CMAKE_CXX_EXTENSIONS NO CACHE BOOL "")

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Build type" FORCE)
endif()
set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug;Release;RelWithDebInfo;MinSizeRel")

if(ENABLE_INCLUDE_WHAT_YOU_USE)
  find_package(Python REQUIRED)
  find_program(iwyu iwyu_tool.py REQUIRED)
  if(WIN32)
    execute_process(COMMAND ${Python_EXECUTABLE} ${iwyu} -p ${CMAKE_BINARY_DIR})
  else()
    execute_process(COMMAND ${iwyu} -p ${CMAKE_BINARY_DIR})
  endif()
endif()

if(ENABLE_CLANG_TIDY)
  find_program(CLANG_TIDY NAMES clang-tidy-12 clang-tidy-11 clang-tidy-10 clang-tidy REQUIRED)
  set(CMAKE_CXX_CLANG_TIDY ${CLANG_TIDY} "-checks=-*,bugprone-*")
endif()

if(ENABLE_CPPCHECK)
  find_program(CPPCHECK cppcheck REQUIRED)
  set(CMAKE_CXX_CPPCHECK
      ${CPPCHECK} "--language=c++;--std=c++17;--inconclusive;--enable=all;-q;--cppcheck-build-dir=${CMAKE_BINARY_DIR}")
endif()

if(ENABLE_CCACHE)
  find_program(CCACHE ccache REQUIRED)
  set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ${CCACHE})
endif()

if(ENABLE_ROM_TESTS AND BUILD_TESTING)
  enable_testing()
  add_subdirectory(tests/rom)
endif()

if(ENABLE_UNIT_TESTS AND BUILD_TESTING)
  enable_testing()
  add_subdirectory(tests/unit)
endif()

add_subdirectory(src)
