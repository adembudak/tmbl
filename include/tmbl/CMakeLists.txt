add_subdirectory(memory)
add_subdirectory(cpu)

add_library(Gameboy Gameboy.cpp)
target_link_directories(Gameboy PUBLIC .)

target_link_libraries(Gameboy PRIVATE cpu)
target_link_libraries(Gameboy PRIVATE memory)

