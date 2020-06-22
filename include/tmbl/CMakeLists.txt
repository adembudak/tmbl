add_subdirectory(memory)
add_subdirectory(cpu)
add_subdirectory(bus)
add_subdirectory(cartridge)

add_library(Gameboy Gameboy.cpp)
target_link_directories(Gameboy PUBLIC .)

target_link_libraries(Gameboy PRIVATE cpu)
target_link_libraries(Gameboy PRIVATE memory)
target_link_libraries(Gameboy PRIVATE bus)
target_link_libraries(Gameboy PRIVATE cartridge)

