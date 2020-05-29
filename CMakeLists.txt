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

add_executable(output main.cpp)
target_include_directories(output PUBLIC include/)

add_subdirectory(include)
target_link_libraries(output PUBLIC Gameboy Boost::headers)
target_compile_definitions(output PUBLIC cxx_std_20)

add_definitions(-Wall -Werror -fpermissive)

if(TOMBUL_BUILD_TESTS)
  add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/tests)
endif()
