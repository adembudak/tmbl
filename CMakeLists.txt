cmake_minimum_required(VERSION 3.20.1)

project(
  tombul
  VERSION 0.0.1
  DESCRIPTION "A very WIP GameBoy emulator implementation"
  HOMEPAGE_URL "https://github.com/adembudak/tombul"
  LANGUAGES CXX)

if(PROJECT_IS_TOP_LEVEL)
  include(CTest)
  include(ExternalData)
endif()
include(CMakeDependentOption)

option(ENABLE_INCLUDE_WHAT_YOU_USE "Run iwyu on source" OFF)
option(ENABLE_CLANG_TIDY "Run clang-tidy on source" OFF)
option(ENABLE_CPPCHECK "Run cppcheck on source" OFF)
option(ENABLE_CCACHE "Cache build with ccache" OFF)
option(LOG_PAK_METADATA "Interpret and print out cartridge header" OFF)
option(ENABLE_UNIT_TESTS "Build unit tests" OFF)

option(ENABLE_ROM_TESTS "Run test ROMs" OFF)
option(BLARGG_TEST_ROMs "Blargg test ROMs" OFF)
option(CGB_ACID2_TEST_ROM "CGB Acid2 test ROM" OFF)
option(DMG_ACID2_TEST_ROM "DMG Acid2 test ROM" OFF)
option(GAMBATTE_TEST_ROMs "Gambette test ROM" OFF)
option(MEALYBUG_TEAROOM_TEST_ROMs "Mealybug teardown test ROMs" OFF)
option(MOONEYE_GB_TEST_ROMs "Mooneye GB ROMs" OFF)

cmake_dependent_option(ENABLE_UNIT_TESTS "" ON BUILD_TESTING OFF)
cmake_dependent_option(ENABLE_ROM_TESTS "" ON BUILD_TESTING OFF)
cmake_dependent_option(BLARGG_TEST_ROMs "" ON ENABLE_ROM_TESTS OFF)
cmake_dependent_option(CGB_ACID2_TEST_ROM "" ON ENABLE_ROM_TESTS OFF)
cmake_dependent_option(DMG_ACID2_TEST_ROM "" ON ENABLE_ROM_TESTS OFF)
cmake_dependent_option(GAMBATTE_TEST_ROMs "" ON ENABLE_ROM_TESTS OFF)
cmake_dependent_option(MEALYBUG_TEAROOM_TEST_ROMs "" ON ENABLE_ROM_TESTS OFF)
cmake_dependent_option(MOONEYE_GB_TEST_ROMs "" ON ENABLE_ROM_TESTS OFF)

set(CMAKE_CXX_STANDARD 17 CACHE STRING "")
set(CMAKE_CXX_STANDARD_REQUIRED YES CACHE BOOL "")
set(CMAKE_CXX_EXTENSIONS NO CACHE BOOL "")

set(tmbl_SOURCE_DIR ${PROJECT_SOURCE_DIR})
set(tmbl_BINARY_DIR ${PROJECT_BINARY_DIR})

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

if(ENABLE_ROM_TESTS)
  enable_testing()
  add_subdirectory(tests/rom)
endif()

if(ENABLE_UNIT_TESTS)
  enable_testing()
  add_subdirectory(tests/unit)
endif()

add_subdirectory(src)
add_subdirectory(bootrom)
