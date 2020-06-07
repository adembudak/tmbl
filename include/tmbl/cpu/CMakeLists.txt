add_subdirectory(registers)
add_subdirectory(clock)

add_library(cpu cpu.cpp)
target_include_directories(cpu PUBLIC .)
target_link_libraries(cpu PRIVATE reg8 reg16 clock)
