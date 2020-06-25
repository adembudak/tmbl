add_subdirectory(cpu)
add_subdirectory(ppu)
add_subdirectory(bus)
add_subdirectory(cartridge)

add_library(Gameboy Gameboy.cpp)
target_include_directories(Gameboy PUBLIC .)

target_link_libraries(Gameboy PRIVATE cpu ppu bus cartridge)
