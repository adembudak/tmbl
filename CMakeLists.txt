cmake_minimum_required(VERSION 3.17)

project(
  tombul
  DESCRIPTION "A GameBoy Emulation"
  HOMEPAGE_URL https://github.com/p1v0t/tombul
  LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(TOMBUL_BUILD_TESTS "Build tests" ON)

include(CTest)
enable_testing()
find_package(Boost QUIET REQUIRED)

find_program(CCACHE ccache)
if(CCACHE)
  set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
endif()

add_subdirectory(include)

add_definitions(-Wall -Werror -fpermissive -Wconversion)

if(TOMBUL_BUILD_TESTS)
  add_subdirectory(tests)
endif()
