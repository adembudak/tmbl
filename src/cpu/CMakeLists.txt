add_subdirectory(internals)

add_library(cpu cpu.cpp)
target_include_directories(cpu PUBLIC ../../include)

target_link_libraries(cpu PRIVATE cpu_internals)
