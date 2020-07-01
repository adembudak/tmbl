cmake_minimum_required(VERSION 3.17)

project(
  tombul
  DESCRIPTION "A GameBoy Emulation"
  HOMEPAGE_URL https://github.com/p1v0t/tombul
  LANGUAGES CXX)

include(CTest)
enable_testing()

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(TOMBUL_BUILD_TESTS "Build tests" OFF)
option(TOMBUL_BUILD_WITH_SDL_FRONTEND
       "Build if you want to see something playable." OFF)
option(TOMBUL_LINT_WHEN_BUILDING "Run linter when building source" OFF)
option(TOMBUL_CACHE_BUILD "Cache build artifacts" OFF)

add_subdirectory(include)
add_definitions(-Wall -Werror -fpermissive -Wconversion)

if(TOMBUL_LINT_WHEN_BUILDING)
  include(cmake/clang_tidy.cmake)
endif()

if(TOMBUL_CACHE_BUILD)
  include(cmake/ccache.cmake)
endif()

if(TOMBUL_BUILD_TESTS)
  add_subdirectory(tests)
endif()

if(TOMBUL_BUILD_WITH_SDL_FRONTEND)
  add_subdirectory(frontends)
endif()
