cmake_minimum_required(VERSION 3.17)
project(
  tombul
  DESCRIPTION "A GameBoy emulation implementation."
  HOMEPAGE_URL https://github.com/p1v0t/tombul
  LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17 CACHE STRING "")
set(CMAKE_CXX_STANDARD_REQUIRED YES CACHE BOOL "")
set(CMAKE_CXX_EXTENSIONS NO CACHE BOOL "")
set(CMAKE_EXPORT_COMPILE_COMMANDS YES)

add_subdirectory(src)
