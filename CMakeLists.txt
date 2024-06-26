cmake_minimum_required(VERSION 3.21.1)

project(
  tombul
  VERSION 0.0.1
  DESCRIPTION "A very WIP GameBoy emulator implementation"
  HOMEPAGE_URL "https://github.com/adembudak/tombul"
  LANGUAGES CXX)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

if(PROJECT_IS_TOP_LEVEL)
  include(CTest)
  include(ExternalData)
  include(FetchContent)
endif()
include(CMakeDependentOption)

option(ENABLE_CCACHE               "Cache build with ccache"    OFF)

option(LOG_PAK_METADATA            "Interpret cartridge header" OFF)
option(ENABLE_UNIT_TESTS           "Build unit tests"           OFF)

option(ENABLE_ROM_TESTS            "Run test ROMs"              OFF)
option(BLARGG_TEST_ROMs            "Blargg test ROMs"           OFF)
option(CGB_ACID2_TEST_ROM          "CGB Acid2 test ROM"         OFF)
option(DMG_ACID2_TEST_ROM          "DMG Acid2 test ROM"         OFF)
option(GAMBATTE_TEST_ROMs          "Gambette test ROM"          OFF)
option(MEALYBUG_TEAROOM_TEST_ROMs  "Mealybug tearoom test ROMs" OFF)
option(MOONEYE_GB_TEST_ROMs        "Mooneye GB ROMs"            OFF)

cmake_dependent_option(ENABLE_UNIT_TESTS          "" ON BUILD_TESTING    OFF)
cmake_dependent_option(ENABLE_ROM_TESTS           "" ON BUILD_TESTING    OFF)
cmake_dependent_option(BLARGG_TEST_ROMs           "" ON ENABLE_ROM_TESTS OFF)
cmake_dependent_option(CGB_ACID2_TEST_ROM         "" ON ENABLE_ROM_TESTS OFF)
cmake_dependent_option(DMG_ACID2_TEST_ROM         "" ON ENABLE_ROM_TESTS OFF)
cmake_dependent_option(GAMBATTE_TEST_ROMs         "" ON ENABLE_ROM_TESTS OFF)
cmake_dependent_option(MEALYBUG_TEAROOM_TEST_ROMs "" ON ENABLE_ROM_TESTS OFF)
cmake_dependent_option(MOONEYE_GB_TEST_ROMs       "" ON ENABLE_ROM_TESTS OFF)

set(tmbl_SOURCE_DIR ${PROJECT_SOURCE_DIR})
set(tmbl_BINARY_DIR ${PROJECT_BINARY_DIR})

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Build type" FORCE)
endif()
set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug;Release;RelWithDebInfo;MinSizeRel")

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

  if(NOT TARGET GTest::gtest_main)
    set(FETCHCONTENT_BASE_DIR ${tmbl_SOURCE_DIR}/external)
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

    fetchcontent_declare(googletest
      URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip)
    fetchcontent_makeavailable(googletest)
  endif()

  add_subdirectory(tests/unit)
endif()

add_subdirectory(src)
