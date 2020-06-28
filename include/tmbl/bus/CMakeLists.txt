add_library(bus bus.cpp)
target_include_directories(bus PUBLIC .)

target_link_libraries(bus PRIVATE cartridge)
target_link_libraries(bus PRIVATE ppu)
target_link_directories(bus PUBLIC ..)
