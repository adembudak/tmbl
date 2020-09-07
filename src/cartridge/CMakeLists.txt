add_subdirectory(rom)
add_subdirectory(mbc1)

add_library(cartridge cartridge.cpp)
target_include_directories(cartridge PUBLIC ../../include)

