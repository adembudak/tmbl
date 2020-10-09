cmake_minimum_required(VERSION 3.17)
project(
  tombul
  DESCRIPTION "A GameBoy emulation implementation."
  HOMEPAGE_URL https://github.com/p1v0t/tombul
  LANGUAGES CXX)

option(ENABLE_INCLUDE_WHAT_YOU_USE "Apply iwyu to source" OFF)

set(CMAKE_CXX_STANDARD 17 CACHE STRING "")
set(CMAKE_CXX_STANDARD_REQUIRED YES CACHE BOOL "")
set(CMAKE_CXX_EXTENSIONS NO CACHE BOOL "")
set(CMAKE_EXPORT_COMPILE_COMMANDS YES)

find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
  set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CCACHE_PROGRAM}")
endif()

if(ENABLE_INCLUDE_WHAT_YOU_USE)
  find_package (Python)
  find_program(iwyu_FOUND iwyu_tool.py)
    if(Python_Interpreter_FOUND AND iwyu_FOUND)
     if (WIN32)
	   execute_process(COMMAND ${Python_EXECUTABLE} iwyu_tool.py -p ${CMAKE_BINARY_DIR})
	 else()
	   execute_process(COMMAND iwyu_tool.py -p ${CMAKE_BINARY_DIR})
     endif()
    endif()
endif()

add_subdirectory(src)
