cmake_minimum_required(VERSION 3.17)
project(
  tombul
  DESCRIPTION "A GameBoy emulation implementation."
  HOMEPAGE_URL https://github.com/p1v0t/tombul
  LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(src)
