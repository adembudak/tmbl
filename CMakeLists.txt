cmake_minimum_required(VERSION 3.17)

project(
  tombul
  DESCRIPTION "A GameBoy emulator implementation"
  HOMEPAGE_URL https://github.com/adembudak/tombul
  LANGUAGES CXX)

option(ENABLE_INCLUDE_WHAT_YOU_USE "Apply iwyu to source" OFF)
option(ENABLE_CLANG_TIDY "Run clang-tidy" OFF)
option(ENABLE_BUILD_CACHE "Cache build with ccache" OFF)
option(ENABLE_ROM_TESTS "Run test ROMs" OFF)
option(ENABLE_UNIT_TESTS "Build unit tests" OFF)

include(CTest)
include(ExternalProject)

set(CMAKE_CXX_STANDARD 17 CACHE STRING "")
set(CMAKE_CXX_STANDARD_REQUIRED YES CACHE BOOL "")
set(CMAKE_CXX_EXTENSIONS NO CACHE BOOL "")
set(CMAKE_EXPORT_COMPILE_COMMANDS YES)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Build type" FORCE)
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release")
endif()

if(ENABLE_INCLUDE_WHAT_YOU_USE)
  find_package(Python)
  find_program(iwyu_FOUND iwyu_tool.py)
  if(Python_Interpreter_FOUND AND iwyu_FOUND)
    if(WIN32)
      execute_process(COMMAND ${Python_EXECUTABLE} iwyu_tool.py -p ${CMAKE_BINARY_DIR})
    else()
      execute_process(COMMAND iwyu_tool.py -p ${CMAKE_BINARY_DIR})
    endif()
  endif()
endif()

if(ENABLE_CLANG_TIDY)
  find_program(CLANG_TIDY NAMES clang-tidy-12 clang-tidy-11 clang-tidy-10 clang-tidy)
  if(CLANG_TIDY)
    set(CMAKE_CXX_CLANG_TIDY ${CLANG_TIDY} "-checks=-*,bugprone-*")
  endif()
endif()

if(ENABLE_BUILD_CACHE)
  find_program(CCACHE_EXECUTABLE ccache)
  if(CCACHE_EXECUTABLE)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ${CCACHE_PROGRAM})
  else()
    message(NOTICE "ccache executable not found, build will not be cached")
  endif()
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
