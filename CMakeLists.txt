cmake_minimum_required(VERSION 3.17)
project(tryouts LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(output main.cpp)
target_include_directories(output PUBLIC include/)
add_definitions(-fcoroutines -Wall -Werror)
